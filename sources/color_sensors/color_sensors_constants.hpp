#pragma once

#include <array>
#include <cstdint>

#include "aliases.hpp"

namespace constants::color_sensor{

    // command bit (0x80) + auto increment bit (0x20)
    constexpr physical::Register CommandBit{0xa0};

    constexpr physical::Register Enable           {0x00 | CommandBit};
    constexpr physical::Register IntegrationTime  {0x01 | CommandBit};
    constexpr physical::Register Control          {0x0f | CommandBit};
    constexpr physical::Register Id               {0x12 | CommandBit};
    constexpr physical::Register ClearDataLow     {0x14 | CommandBit};

    // configuration
    constexpr uint8_t TotalSensorCount      {16};
    constexpr uint8_t SensorsPerMux         {8};
    constexpr uint8_t EnableValue           {0x03}; // PON (bit 0) and AEN (bit 1)
    constexpr uint8_t PowerOnOnlyValue      {0x01}; // PON only
    constexpr uint8_t IntegrationTimeValue  {0xfd}; // ~ms
    constexpr uint8_t GainValue             {0x02}; // 16x gain
    constexpr uint8_t RgbcDataByteCount     {8};

    // delay
    constexpr units::Us MinimumReadInterval{50000ULL};
    // after using the new classification method, it can be safely reduce to around 12.5 ms 
    // while still maintaining a 0% error rate. I pushed it a bit further to 10 ms.
    constexpr units::Us SeekStateStopToSamplingSettleDelay{10000ULL}; // 25000
    constexpr units::Us TotalIntegrationTime{ // T_int = (256 - ATIME) * 2.4 ms
        (static_cast<uint32_t>(0x100U) - static_cast<uint32_t>(IntegrationTimeValue)) * 2400U
    };

    // constexpr units::Us MuxChannelSamplingSettleDelay{50ULL};

    // mux
    constexpr uint8_t MuxDisableMask{0x00};

    // color threshold
    constexpr uint16_t BlackClearThreshold      {200};
    constexpr float WhiteSaturationThreshold    {.25f};
    constexpr float YellowRedRatioThreshold     {1.5f};
    constexpr float YellowGreenRatioThreshold   {1.5f};

    // normalized distance metric
    constexpr uint32_t NormalizedScaleFactor{10000U};
    constexpr uint32_t MinimumSafeDivisor{1U};
    constexpr uint32_t BrightnessDistanceWeight{2U};
    constexpr uint16_t MaximumColorCost{65535U};
    constexpr uint16_t MinimumColorCost{0U};

    // others
    constexpr int FrontToBackDistance{5};
    constexpr std::array<uint8_t, TotalSensorCount> SensorIndexMap{
        0, 8, 1, 9, 2, 10, 3, 11, 4, 12, 5, 13, 6, 14, 7, 15
    };

} // namespace constants::color_sensor
