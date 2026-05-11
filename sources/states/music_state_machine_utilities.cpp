#include "music_state_machine.hpp"

void MusicStateMachine::queueNoteOn(
    uint8_t instrumentChannel,
    units::midi::Channel midiChannel,
    units::midi::Note note,
    CommandBuffer &queue
){
    queue.push(midi_command::QueuedNoteOn{
        .channel = midiChannel,
        .note = note,
        .velocity = static_cast<units::midi::Velocity>(
            (states_.volumes[instrumentChannel] * constants::midi::MaximumVelocity) / 100.0f
        )
    });
}

void MusicStateMachine::queueNoteOff(
    units::midi::Channel midiChannel,
    units::midi::Note note,
    CommandBuffer &queue
){
    queue.push(midi_command::QueuedNoteOff{
        .channel = midiChannel,
        .note = note
    });
}

void MusicStateMachine::clearActiveNotes(
    uint8_t instrumentChannel,
    units::midi::Channel midiChannel,
    CommandBuffer &queue
){
    const auto activeNotes{states_.activeNotes[instrumentChannel].toVector()};
    for(const auto &activeNote : activeNotes){
        queueNoteOff(midiChannel, static_cast<units::midi::Note>(activeNote), queue);
    }

    states_.activeNotes[instrumentChannel].clear();
}

void MusicStateMachine::applyArticulation(
    uint8_t instrumentChannel,
    units::Articulation articulation,
    CommandBuffer &queue
){
    // if(states_.articulations[channelIndex] == articulation){
    //     return;
    // }
    const auto midiChannel{
        states_.instruments[instrumentChannel] == instruments::Subset::Drum_Sets 
            ? constants::midi::DrumSetsChannel : instrumentChannel
    };

    clearActiveNotes(instrumentChannel, midiChannel, queue);
    states_.articulations[instrumentChannel] = articulation;
}