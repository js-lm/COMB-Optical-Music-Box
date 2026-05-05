#include "sensors_manager.hpp"

#include "constants.hpp"
#include "debug_utilities.hpp"

#include <hardware/i2c.h>
#include <pico/stdlib.h>
#include <stdio.h>

void SensorsManager::initializeSensors(){
    DEBUG_PRINT("Initializing color sensors...");
    
    int successCount{0};
    
    for(color_sensor_data::SensorIndex sensorIndex{0}; 
        sensorIndex < constants::color_sensor::TotalSensorCount; 
        sensorIndex++
    ){
        selectSensorMuxChannel(sensorIndex);

        writeColorSensorRegister(constants::color_sensor::Enable, constants::color_sensor::EnableValue);
        writeColorSensorRegister(constants::color_sensor::IntegrationTime, constants::color_sensor::IntegrationTimeValue);
        writeColorSensorRegister(constants::color_sensor::Control, constants::color_sensor::GainValue);
    }

    isSamplingActive_ = false;
    samplingStartTimestamp_ = time_us_64();

    DEBUG_PRINT("Color sensors initialized!");
}

void SensorsManager::setSensorEnabled(color_sensor_data::SensorIndex sensorIndex, bool enabled){
    selectSensorMuxChannel(sensorIndex);

    uint8_t enableValue{enabled ? constants::color_sensor::EnableValue : constants::color_sensor::PowerOnOnlyValue};
    writeColorSensorRegister(constants::color_sensor::Enable, enableValue);
}

void SensorsManager::startSampling(){
    // for(color_sensor_data::SensorIndex sensorIndex{0}; 
    //     sensorIndex < constants::color_sensor::TotalSensorCount; 
    //     sensorIndex++
    // ){
    //     setSensorEnabled(sensorIndex, true);
    // }

    samplingStartTimestamp_ = time_us_64();
    isSamplingActive_ = true;
}

void SensorsManager::stopSampling(){
    // for(color_sensor_data::SensorIndex sensorIndex{0}; 
    //     sensorIndex < constants::color_sensor::TotalSensorCount; 
    //     sensorIndex++
    // ){
    //     setSensorEnabled(sensorIndex, false);
    // }

    isSamplingActive_ = false;
}

bool SensorsManager::isSamplingReady() const{
    if(!isSamplingActive_) return false;

    // constexpr units::Us samplingWarmupDelay{integrationTime + constants::color_sensor::SeekStateStopToSamplingSettleDelay};

    const units::TimestampUs currentTime{time_us_64()};
    return currentTime - samplingStartTimestamp_ >= constants::color_sensor::TotalIntegrationTime;
}

SensorsManager::ColorRow SensorsManager::collectSensorData(){
    // front sensors
    ColorDataRow colorDataRow{};
    for(color_sensor_data::SensorIndex sensorIndex{0}; 
        sensorIndex < constants::color_sensor::SensorsPerMux; 
        sensorIndex++
    ){
        const auto colorData{getColorData(sensorIndex, readSensorRGBC(sensorIndex))};
        colorDataRow.colorRow[sensorIndex] = colorData.mostLikelyColor;
        colorDataRow.costRow[sensorIndex] = colorData.costs;
    }
    colorDataRowQueue_.push(colorDataRow);

    if(colorDataRowQueue_.size() <= constants::color_sensor::FrontToBackDistance) return {};

    ColorDataRow latestColorDataRow{colorDataRowQueue_.pop()};

    // back sensors
    for(color_sensor_data::SensorIndex sensorIndex{constants::color_sensor::SensorsPerMux}; 
        sensorIndex < constants::color_sensor::TotalSensorCount; 
        sensorIndex++
    ){
        const auto colorData{getColorData(sensorIndex, readSensorRGBC(sensorIndex))};
        latestColorDataRow.colorRow[sensorIndex] = colorData.mostLikelyColor;
        latestColorDataRow.costRow[sensorIndex] = colorData.costs;
    }

    auto checksumResidue{checksum(latestColorDataRow)};
    return checksumResidue == 0 ? latestColorDataRow.colorRow : trySoftCorrection(latestColorDataRow, checksumResidue);
}

SensorsManager::DEBUG_RawColorReadingRow SensorsManager::DEBUG_collectSensorRawReadings(){
    DEBUG_RawColorReadingRow colorRow{};

    for(color_sensor_data::SensorIndex sensorIndex{0}; 
        sensorIndex < constants::color_sensor::TotalSensorCount; 
        sensorIndex++
    ){
        colorRow[sensorIndex] = readSensorRGBC(sensorIndex);
    }

    return colorRow;
}

SensorsManager::DEBUG_CompleteColorDataRow SensorsManager::DEBUG_getCompleteColorDataRow(){
    DEBUG_CompleteColorDataRow debugRow{};

    struct DebugFrontFrame{
        ColorDataRow colorDataRow{};
        DEBUG_RawColorReadingRow rawColorReadingRow{};
    };

    static utilities::RingBuffer<DebugFrontFrame, constants::color_sensor::FrontToBackDistance + 1> queuedFrontFrames{};

    DebugFrontFrame frontFrame{};

    // for(color_sensor_data::SensorIndex sensorIndex{constants::color_sensor::SensorsPerMux}; 
    //     sensorIndex --> 0;
    // ){

    for(color_sensor_data::SensorIndex sensorIndex{0}; 
        sensorIndex < constants::color_sensor::SensorsPerMux; 
        sensorIndex++
    ){
        const auto rawColorReading{readSensorRGBC(sensorIndex)};
        const auto colorData{getColorData(sensorIndex, rawColorReading)};

        frontFrame.colorDataRow.colorRow[sensorIndex] = colorData.mostLikelyColor;
        frontFrame.colorDataRow.costRow[sensorIndex] = colorData.costs;
        frontFrame.rawColorReadingRow[sensorIndex] = rawColorReading;
    }

    queuedFrontFrames.push(frontFrame);

    if(queuedFrontFrames.size() <= constants::color_sensor::FrontToBackDistance){
        return debugRow;
    }

    const auto alignedFrontFrame{queuedFrontFrames.pop()};
    ColorDataRow colorDataRow{alignedFrontFrame.colorDataRow};

    // for(color_sensor_data::SensorIndex sensorIndex{constants::color_sensor::SensorsPerMux}; 
    //     sensorIndex --> 0;
    // ){
    for(color_sensor_data::SensorIndex sensorIndex{0}; 
        sensorIndex < constants::color_sensor::SensorsPerMux; 
        sensorIndex++
    ){
        debugRow.sensorDataRow[sensorIndex].rawColorReading = alignedFrontFrame.rawColorReadingRow[sensorIndex];
        debugRow.sensorDataRow[sensorIndex].detectedColor = colorDataRow.colorRow[sensorIndex];
    }

    for(color_sensor_data::SensorIndex sensorIndex{constants::color_sensor::SensorsPerMux}; 
        sensorIndex < constants::color_sensor::TotalSensorCount; 
        sensorIndex++
    ){
        const auto rawColorReading{readSensorRGBC(sensorIndex)};
        const auto colorData{getColorData(sensorIndex, rawColorReading)};

        colorDataRow.colorRow[sensorIndex] = colorData.mostLikelyColor;
        colorDataRow.costRow[sensorIndex] = colorData.costs;

        debugRow.sensorDataRow[sensorIndex].rawColorReading = rawColorReading;
        debugRow.sensorDataRow[sensorIndex].detectedColor = colorData.mostLikelyColor;
    }

    debugRow.finalColorRow = colorDataRow.colorRow;

    debugRow.checksumResidue = checksum(colorDataRow);
    debugRow.isChecksumValid = debugRow.checksumResidue == 0;

    if(debugRow.isChecksumValid) return debugRow;

    debugRow.finalColorRow = trySoftCorrection(colorDataRow, debugRow.checksumResidue);

    for(color_sensor_data::SensorIndex sensorIndex{0}; 
        sensorIndex < constants::color_sensor::TotalSensorCount; 
        sensorIndex++
    ){
        const auto originalColor{colorDataRow.colorRow[sensorIndex]};
        const auto correctedColor{debugRow.finalColorRow[sensorIndex]};

        if(originalColor == correctedColor) continue;

        debugRow.correctionApplied = true;
        debugRow.correctedSensorIndex = sensorIndex;
        debugRow.correctedFromColor = originalColor;
        debugRow.correctedToColor = correctedColor;
        debugRow.correctedFromCost = colorDataRow.costRow[sensorIndex][originalColor];
        debugRow.correctedToCost = colorDataRow.costRow[sensorIndex][correctedColor];
        break;
    }

    return debugRow;
}

color_sensor_data::RawColorReading SensorsManager::readSensorRGBC(color_sensor_data::SensorIndex sensorIndex){
    color_sensor_data::RawColorReading result{0, 0, 0, 0};

    selectSensorMuxChannel(sensorIndex);

    // clear: 0x14-0x15, red: 0x16-0x17, green: 0x18-0x19, blue: 0x1A-0x1B
    physical::Register registerAddress{constants::color_sensor::ClearDataLow};
    uint8_t rgbcData[constants::color_sensor::RgbcDataByteCount]{};

    int writeResult{i2c_write_blocking(i2c0, constants::i2c_address::ColorSensor, &registerAddress, 1, true)};
    if(writeResult < 0) return result;
    
    int readResult{i2c_read_blocking(
        i2c0, constants::i2c_address::ColorSensor, rgbcData, constants::color_sensor::RgbcDataByteCount, false
    )};
    if(readResult < 0) return result;

    result.clear = static_cast<uint16_t>(rgbcData[0] | (rgbcData[1] << 8));
    result.red   = static_cast<uint16_t>(rgbcData[2] | (rgbcData[3] << 8));
    result.green = static_cast<uint16_t>(rgbcData[4] | (rgbcData[5] << 8));
    result.blue  = static_cast<uint16_t>(rgbcData[6] | (rgbcData[7] << 8));

    return result;
}