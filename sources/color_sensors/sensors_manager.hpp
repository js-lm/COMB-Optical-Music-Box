#pragma once

#include "aliases.hpp"
#include "constants.hpp"

#include "utilities/ring_buffer.hpp"
#include "utilities/enum_map.hpp"

#include <vector>
#include <queue>
#include <array>
#include <optional>

#include <hardware/i2c.h>

class SensorsManager{
public:
    enum class Color : uint8_t{
        White   = 0,
        Red     = 1,
        Green   = 2,
        Blue    = 3,
        Black   = 4,
        None    = 5,
        Error   = 6
    };

    using ColorRow = std::array<Color, constants::color_sensor::TotalSensorCount>;

private:
    struct MuxChannelInfo{
        physical::I2CAddress muxAddress;
        physical::Channel muxChannel;
    };

    using ColorCost = utilities::EnumMap<Color, uint16_t>;
    using ColorCostRow = std::array<ColorCost, constants::color_sensor::TotalSensorCount>;

    struct ColorData{
        Color mostLikelyColor{Color::Error};
        ColorCost costs{};
    };
    // using ColorDataRow = std::array<ColorData, constants::color_sensor::TotalSensorCount>;

    struct ColorDataRow{
        ColorRow colorRow{};
        ColorCostRow costRow{};
    };

public:
    using DEBUG_RawColorReadingRow = std::array<
        color_sensor_data::RawColorReading, 
        constants::color_sensor::TotalSensorCount
    >;

    struct DEBUG_CompleteColorDataRow{
        struct SensorData{
            color_sensor_data::RawColorReading rawColorReading{};
            Color detectedColor{Color::Error};
        };

        std::array<SensorData, constants::color_sensor::TotalSensorCount> sensorDataRow{};

        ColorRow finalColorRow{};

        bool isChecksumValid{false};
        uint8_t checksumResidue{0};

        bool correctionApplied{false};
        color_sensor_data::SensorIndex correctedSensorIndex{0};
        Color correctedFromColor{Color::Error};
        Color correctedToColor{Color::Error};
        uint16_t correctedFromCost{0};
        uint16_t correctedToCost{0};
    };

private:
    units::TimestampUs lastReadTime_{0};
    units::TimestampUs samplingStartTimestamp_{0};
    bool isSamplingActive_{false};
    
    // std::optional<calibrator::ReferenceProfile> referenceProfile_{};

    utilities::RingBuffer<ColorDataRow, constants::color_sensor::FrontToBackDistance + 1> colorDataRowQueue_{};

public:
    SensorsManager() = default;
    ~SensorsManager() = default;

    void initialize();

    ColorRow collectSensorData();
    void startSampling();
    void stopSampling();
    bool isSamplingReady() const;

public:
    DEBUG_RawColorReadingRow DEBUG_collectSensorRawReadings();
    DEBUG_CompleteColorDataRow DEBUG_getCompleteColorDataRow();

    // void setReferenceProfile(const calibrator::ReferenceProfile &referenceProfile);

private:
    void initializeSensors();
    void setSensorEnabled(color_sensor_data::SensorIndex sensorIndex, bool enabled);

    color_sensor_data::RawColorReading readSensorRGBC(color_sensor_data::SensorIndex sensorIndex);

    ColorData getColorData(color_sensor_data::SensorIndex sensorIndex, const color_sensor_data::RawColorReading &color) const;

private:
    MuxChannelInfo calculateMuxInfo(color_sensor_data::SensorIndex sensorIndex) const;
    void selectSensorMuxChannel(color_sensor_data::SensorIndex sensorIndex);
    void writeColorSensorRegister(physical::Register registerAddress, uint8_t value);

    void selectMuxChannel(i2c_inst_t *i2c, physical::I2CAddress muxAddress, physical::Channel channel);

    uint8_t checksum(const ColorDataRow &colorDataRow) const;
    ColorRow trySoftCorrection(const ColorDataRow &colorDataRow, uint8_t checksumResidue) const;

};