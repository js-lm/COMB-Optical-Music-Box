#pragma once

#include <hardware/uart.h>

#include <cstddef>
#include <array>

#include "aliases.hpp"

#include "color_sensors/color_sensors_constants.hpp"
#include "light_sensor/light_sensor_constants.hpp"
#include "midi_unit/midi_constants.hpp"
#include "motor/motor_constants.hpp"
#include "music_decoder/decoder_constants.hpp"

namespace constants{

    namespace pins{
        constexpr physical::Pin MotorDirection {15};
        constexpr physical::Pin MotorStep      {14};
        constexpr physical::Pin MotorEnable    {13};

        constexpr physical::Pin MuxData  {4};
        constexpr physical::Pin MuxClock {5};

        constexpr physical::Pin MidiTransmit {0};
        constexpr physical::Pin MidiReceive  {1};

        constexpr physical::Pin LightAnalogOut {26};
    } // namespace pins

    namespace adc{
        constexpr physical::Channel LightInputChannel{0};
    } // namespace adc

    namespace i2c{
        constexpr uint32_t Frequency{400000}; // 400 kHz
    } // namespace i2c

    namespace i2c_address{
        constexpr physical::I2CAddress MuxFront  {0x70};
        constexpr physical::I2CAddress MuxBack   {0x71};
        constexpr physical::I2CAddress ColorSensor{0x29};
    } // namespace i2c_address


    namespace system{
        constexpr uint16_t MaximumAdcValue{4095}; // 12 bits

        constexpr uint8_t MaximumCommandsInBuffer{255};

        constexpr uint8_t BaseNoteOffset{23};

        constexpr instruments::Subset   DefaultInstrument{instruments::Subset::Grand_Piano};
        constexpr units::Volume         DefaultVolume{100};
        constexpr units::Articulation   DefaultArticulation{units::Articulation::Normal};
        constexpr units::Tempo          DefaultTempo{100};

        constexpr float MaximumNotesPerSecond{12.0f};
    } // namespace system

    namespace runtime{
        constexpr units::StepsPerSecond MinimumStepRate{1000};
        constexpr units::StepsPerSecond MaximumStepRate{12000};

        constexpr units::midi::Velocity DefaultNoteVelocity{100};
    } // namespace runtime

} // namespace constants