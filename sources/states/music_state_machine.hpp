#pragma once

#include "machine_states.hpp"
#include "music_decoder/decoded_row.hpp"
#include "music_data/midi_command.hpp"

#include "utilities/ring_buffer.hpp"

#include "constants.hpp"
#include "aliases.hpp"

#include <vector>
#include <array>

class MusicStateMachine{
public:
    using CommandBuffer = utilities::RingBuffer<midi_command::Command, constants::system::MaximumCommandsInBuffer>;
    using InstrumentCommands = std::array<music_decoder::InstrumentCommand, constants::decoder::NumberOfInstrumentChannel>;

private:
    MachineStates states_;

public:
    MusicStateMachine() = default;
    ~MusicStateMachine() = default;

    void process(
        const music_decoder::DecodedRow &row, 
        CommandBuffer &queue
    );

    MachineStates &getStates(){ return states_;}
    const MachineStates &getStates() const{ return states_;}

private:
    void updateSystemState(
        const music_decoder::SystemCommand &systemCommand, 
        CommandBuffer &queue
    );
    void evaluateInstrumentCommands(
        const InstrumentCommands &instrumentCommands, 
        CommandBuffer &queue
    );

private:
    void queueNoteOn(
        uint8_t instrumentChannel,
        units::midi::Channel midiChannel,
        units::midi::Note note,
        CommandBuffer &queue
    );

    void queueNoteOff(
        units::midi::Channel channelIndex,
        units::midi::Note note,
        CommandBuffer &queue
    );

    void clearActiveNotes(
        uint8_t instrumentChannel,
        units::midi::Channel midiChannel,
        CommandBuffer &queue
    );

    void applyArticulation(
        units::midi::Channel channelIndex,
        units::Articulation articulation,
        CommandBuffer &queue
    );

private:
    uint16_t decodeBase5(uint8_t digit3, uint8_t digit2, uint8_t digit1){ // TODO: it's duplicated, kinda
        return digit3 * 25 + digit2 * 5 + digit1;
    }
};
