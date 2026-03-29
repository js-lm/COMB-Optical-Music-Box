#include "sensors_manager.hpp"

#include <hardware/i2c.h>
#include <pico/stdlib.h>

#include <algorithm>

#include "constants.hpp"

#include "normalized_reference_profile.hpp"

void SensorsManager::selectMuxChannel(i2c_inst_t *i2c, physical::I2CAddress muxAddress, physical::Channel channel){
    constexpr uint8_t disableMask{constants::color_sensor::MuxDisableMask};

    i2c_write_blocking(i2c, constants::i2c_address::MuxFront, &disableMask, 1, false);
    i2c_write_blocking(i2c, constants::i2c_address::MuxBack,  &disableMask, 1, false);
    
    uint8_t channelMask{static_cast<uint8_t>(1 << channel)};
    i2c_write_blocking(i2c, muxAddress, &channelMask, 1, false);
}

SensorsManager::MuxChannelInfo SensorsManager::calculateMuxInfo(color_sensor_data::SensorIndex sensorIndex) const{
    physical::I2CAddress muxAddress{sensorIndex < constants::color_sensor::SensorsPerMux ? 
        constants::i2c_address::MuxFront : constants::i2c_address::MuxBack
    };
    physical::Channel muxChannel{static_cast<physical::Channel>(
        sensorIndex % constants::color_sensor::SensorsPerMux
    )};
    return MuxChannelInfo{muxAddress, muxChannel};
}

void SensorsManager::selectSensorMuxChannel(color_sensor_data::SensorIndex sensorIndex){
    MuxChannelInfo muxInfo{calculateMuxInfo(sensorIndex)};
    selectMuxChannel(i2c0, muxInfo.muxAddress, muxInfo.muxChannel);
}

void SensorsManager::writeColorSensorRegister(physical::Register registerAddress, uint8_t value){
    uint8_t data[2]{registerAddress, value};
    i2c_write_blocking(i2c0, constants::i2c_address::ColorSensor, data, 2, false);
}

// void SensorsManager::setReferenceProfile(const calibrator::ReferenceProfile &referenceProfile){
//     referenceProfile_ = referenceProfile;
// }

SensorsManager::ColorData SensorsManager::getColorData(
    color_sensor_data::SensorIndex sensorIndex,
    const color_sensor_data::RawColorReading &color
) const{
    ColorData colorData{};

    if(sensorIndex >= constants::color_sensor::TotalSensorCount){
        colorData.mostLikelyColor = Color::Error;
        return colorData;
    }

    const constants::color_sensor::NormalizedBaseValues &normalizedBaseValues{
        constants::color_sensor::PrecomputedNormalizedProfile[sensorIndex]
    };

    if(static_cast<uint32_t>(color.clear) >= normalizedBaseValues.noPaperClearThreshold){
        colorData.costs[Color::White] = constants::color_sensor::MaximumColorCost;
        colorData.costs[Color::Red]   = constants::color_sensor::MaximumColorCost;
        colorData.costs[Color::Green] = constants::color_sensor::MaximumColorCost;
        colorData.costs[Color::Blue]  = constants::color_sensor::MaximumColorCost;
        colorData.costs[Color::Black] = constants::color_sensor::MaximumColorCost;
        colorData.costs[Color::None]  = constants::color_sensor::MinimumColorCost;
        colorData.costs[Color::Error] = constants::color_sensor::MinimumColorCost;

        colorData.mostLikelyColor = Color::None;
        return colorData;
    }

    const uint32_t incomingClearRaw{
        std::max(
            static_cast<uint32_t>(color.clear),
            constants::color_sensor::MinimumSafeDivisor
        )
    };

    const constants::color_sensor::NormalizedColor normalizedIncomingColor{
        (static_cast<uint32_t>(color.red)   * constants::color_sensor::NormalizedScaleFactor) / incomingClearRaw,
        (static_cast<uint32_t>(color.green) * constants::color_sensor::NormalizedScaleFactor) / incomingClearRaw,
        (static_cast<uint32_t>(color.blue)  * constants::color_sensor::NormalizedScaleFactor) / incomingClearRaw,
        (static_cast<uint32_t>(color.clear) * constants::color_sensor::NormalizedScaleFactor) / normalizedBaseValues.noPaperClearRaw
    };

    auto absoluteDifference{[](uint32_t leftValue, uint32_t rightValue)->uint32_t{
        return leftValue >= rightValue ? leftValue - rightValue : rightValue - leftValue;
    }};

    auto hybridDistance{[&absoluteDifference](
        const constants::color_sensor::NormalizedColor &normalizedMeasuredColor,
        const constants::color_sensor::NormalizedColor &normalizedTargetColor
    )->uint32_t{
        const uint32_t redDifference{
            absoluteDifference(normalizedMeasuredColor.red, normalizedTargetColor.red)
        };
        const uint32_t greenDifference{
            absoluteDifference(normalizedMeasuredColor.green, normalizedTargetColor.green)
        };
        const uint32_t blueDifference{
            absoluteDifference(normalizedMeasuredColor.blue, normalizedTargetColor.blue)
        };
        const uint32_t brightnessDifference{
            absoluteDifference(normalizedMeasuredColor.clear, normalizedTargetColor.clear)
        };

        const uint32_t weightedBrightnessDifference{
            brightnessDifference * constants::color_sensor::BrightnessDistanceWeight
        };

        const uint32_t totalDistance{
            redDifference + greenDifference + blueDifference + weightedBrightnessDifference
        };

        const uint32_t cappedDistance{
            std::min(
                totalDistance,
                static_cast<uint32_t>(constants::color_sensor::MaximumColorCost)
            )
        };

        return cappedDistance;
    }};

    colorData.costs[Color::White] = hybridDistance(normalizedIncomingColor, normalizedBaseValues.white);
    colorData.costs[Color::Red]   = hybridDistance(normalizedIncomingColor, normalizedBaseValues.red);
    colorData.costs[Color::Green] = hybridDistance(normalizedIncomingColor, normalizedBaseValues.green);
    colorData.costs[Color::Blue]  = hybridDistance(normalizedIncomingColor, normalizedBaseValues.blue);
    colorData.costs[Color::Black] = hybridDistance(normalizedIncomingColor, normalizedBaseValues.black);
    colorData.costs[Color::None]  = constants::color_sensor::MaximumColorCost;
    colorData.costs[Color::Error] = constants::color_sensor::MinimumColorCost;

    Color closestColor{Color::Error};
    auto minimumDistance{constants::color_sensor::MaximumColorCost};

    constexpr std::array<Color, constants::decoder::Radix> evaluatedColors{
        Color::White,
        Color::Red,
        Color::Green,
        Color::Blue,
        Color::Black
    };

    for(const Color evaluatedColor : evaluatedColors){
        const auto currentDistance{colorData.costs[evaluatedColor]};
        if(currentDistance < minimumDistance){
            minimumDistance = currentDistance;
            closestColor = evaluatedColor;
        }
    }

    colorData.mostLikelyColor = closestColor;

    return colorData;
}

uint8_t SensorsManager::checksum(const ColorDataRow &colorDataRow) const{
    uint8_t sum{0};

    for(uint8_t colorDataIndex{0}; 
        colorDataIndex < constants::color_sensor::TotalSensorCount; 
        colorDataIndex++
    ){
        // sum += static_cast<uint8_t>(colorDataRow[colorDataIndex].mostLikelyColor);
        sum += static_cast<uint8_t>(colorDataRow.colorRow[colorDataIndex]);
    }

    return sum % constants::decoder::Radix;
}

SensorsManager::ColorRow SensorsManager::trySoftCorrection(const ColorDataRow &colorDataRow, uint8_t checksumResidue) const{
    ColorRow correctedRow{colorDataRow.colorRow};

    if(checksumResidue == 0) return correctedRow; 

    uint8_t requiredAddition{static_cast<uint8_t>((constants::decoder::Radix - checksumResidue) % constants::decoder::Radix)};

    uint8_t bestSensorIndex{0};
    uint16_t lowestPenaltyDelta{std::numeric_limits<uint16_t>::max()};
    Color bestAlternativeColor{Color::Error};

    for(uint8_t sensorIndex{0}; sensorIndex < constants::color_sensor::TotalSensorCount; sensorIndex++){
        Color currentColor{colorDataRow.colorRow[sensorIndex]};
        
        if(currentColor >= Color::None) continue;

        uint8_t currentValue{static_cast<uint8_t>(currentColor)};
        uint8_t alternativeValue{static_cast<uint8_t>((currentValue + requiredAddition) % constants::decoder::Radix)};
        Color alternativeColor{static_cast<Color>(alternativeValue)};

        uint16_t currentCost{colorDataRow.costRow[sensorIndex][currentColor]};
        uint16_t alternativeCost{colorDataRow.costRow[sensorIndex][alternativeColor]};

        auto penaltyDelta{alternativeCost > currentCost ? alternativeCost - currentCost : 0};

        if(penaltyDelta < lowestPenaltyDelta){
            lowestPenaltyDelta = penaltyDelta;
            bestSensorIndex = sensorIndex;
            bestAlternativeColor = alternativeColor;
        }
    }

    if(bestAlternativeColor != Color::Error) correctedRow[bestSensorIndex] = bestAlternativeColor;

    return correctedRow;
}