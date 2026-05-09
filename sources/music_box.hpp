#pragma once

#include "motor/motor_manager.hpp"
#include "light_sensor/light_manager.hpp"
#include "color_sensors/sensors_manager.hpp"
#include "midi_unit/midi_manager.hpp"
#include "music_decoder/music_decoder.hpp"
#include "states/music_state_machine.hpp"

#include "utilities/enum_map.hpp"

#include <atomic>

class MusicBox{
private:
    MotorManager        motorManager_{};
    LightSensorManager  lightSensorManager_{};
    SensorsManager      sensorsManager_{};
    MidiManager         midiManager_{};
    MusicDecoder        musicDecoder_{};
    MusicStateMachine   musicStateMachine_{};

    // inline static std::atomic<MotorManager*> motorManagerPointer_{nullptr};

private:
    units::TimestampUs lastUpdateTime_{0};
    units::Us timeSinceLastStep_{0};
    units::TimestampUs motorStopTimestamp_{0};
    units::TimestampUs lastNoteTimestamp_{0};

private:
    // enum class Channel : uint8_t{ None, Instrument_1, Instrument_2, Instrument_3, Chord};
    // utilities::EnumMap<Channel, midi_data::Instrument> channelInstruments_{};

private:
    midi_command::MachineState commandState_{};
    utilities::RingBuffer<midi_command::Command, constants::system::MaximumCommandsInBuffer> commandQueue_{};

private:
    // calibrator::ReferenceProfile calibrationProfile_{};

public:
    static MusicBox &getInstance();

    int run();

private:
    MusicBox() = default;
    ~MusicBox() = default;

private:
    void initialize();
    void update();

    static void core1Entry();

private:
    void nextState();
    void updateTimers();

private:
    void updateCalibrateMarkersState();
    void updateCalibrateSensorsState();

    void updateConfigureMetadataState();
    void updateConfigureInstrumentsState();
    
    void updateSeekState();
    void updateSamplingState();
    void updateWaitState();
    void updateProcessState();

    void executeNextBufferedCommand();

private:
    void DEBUG_bitFlipTester();
    void DEBUG_updateProcessState();
};