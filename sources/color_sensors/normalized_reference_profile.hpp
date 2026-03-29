#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

#include "reference_profile.hpp"
#include "constants.hpp"

namespace constants::color_sensor{

    struct NormalizedColor{
        uint32_t red    {0};
        uint32_t green  {0};
        uint32_t blue   {0};
        uint32_t clear  {0};
    };

    struct NormalizedBaseValues{
        NormalizedColor white{};
        NormalizedColor red{};
        NormalizedColor green{};
        NormalizedColor blue{};
        NormalizedColor black{};

        uint32_t noPaperClearRaw{0};
        uint32_t noPaperClearThreshold{0};
    };

    namespace _{

        consteval uint32_t ensureMinimumValue(uint32_t value, uint32_t minimumValue){
            if(value < minimumValue) return minimumValue;
            return value;
        }

        consteval uint32_t calculateNoPaperThreshold(
            uint32_t whiteClearValue,
            uint32_t noPaperClearValue
        ){
            return whiteClearValue + ((noPaperClearValue - whiteClearValue) / 2U);
        }

        consteval NormalizedColor normalizeReading(
            const color_sensor_data::RawColorReading &rawColorReading,
            uint32_t noPaperClearRawValue
        ){
            constexpr uint32_t normalizedScaleFactor{constants::color_sensor::NormalizedScaleFactor};
            constexpr uint32_t minimumDivisor{1U};

            const uint32_t clearChannelValue{
                ensureMinimumValue(static_cast<uint32_t>(rawColorReading.clear), minimumDivisor)
            };

            const uint32_t safeNoPaperClearRawValue{
                ensureMinimumValue(noPaperClearRawValue, minimumDivisor)
            };

            return NormalizedColor{
                (static_cast<uint32_t>(rawColorReading.red) * normalizedScaleFactor) / clearChannelValue,
                (static_cast<uint32_t>(rawColorReading.green) * normalizedScaleFactor) / clearChannelValue,
                (static_cast<uint32_t>(rawColorReading.blue) * normalizedScaleFactor) / clearChannelValue,
                (static_cast<uint32_t>(rawColorReading.clear) * normalizedScaleFactor) / safeNoPaperClearRawValue
            };
        }

        consteval NormalizedBaseValues normalizeBaseValues(const BaseValues &baseValues){
            constexpr uint32_t minimumDivisor{1U};

            const uint32_t noPaperClearRawValue{
                ensureMinimumValue(static_cast<uint32_t>(baseValues.noPaper.clear), minimumDivisor)
            };

            return NormalizedBaseValues{
                normalizeReading(baseValues.white, noPaperClearRawValue),
                normalizeReading(baseValues.red, noPaperClearRawValue),
                normalizeReading(baseValues.green, noPaperClearRawValue),
                normalizeReading(baseValues.blue, noPaperClearRawValue),
                normalizeReading(baseValues.black, noPaperClearRawValue),
                noPaperClearRawValue,
                calculateNoPaperThreshold(
                    static_cast<uint32_t>(baseValues.white.clear),
                    noPaperClearRawValue
                )
            };
        }

    } // namespace _

    consteval std::array<NormalizedBaseValues, constants::color_sensor::TotalSensorCount> computeNormalizedProfile(){
        std::array<NormalizedBaseValues, constants::color_sensor::TotalSensorCount> normalizedProfile{};

        for(std::size_t sensorIndex{0};
            sensorIndex < constants::color_sensor::TotalSensorCount;
            sensorIndex++
        ){
            normalizedProfile[sensorIndex] = _::normalizeBaseValues(
                BaseProfile[sensorIndex]
            );
        }

        return normalizedProfile;
    }

    inline constexpr std::array<
        NormalizedBaseValues, constants::color_sensor::TotalSensorCount
    > PrecomputedNormalizedProfile{computeNormalizedProfile()};

    static_assert(
        PrecomputedNormalizedProfile[0].noPaperClearRaw >= 1U,
        "invalid no paper clear baseline"
    );

} // namespace calibrator