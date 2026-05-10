#pragma once

#include "aliases.hpp"

#include "midi_unit/midi_manager.hpp"

#include <variant>
#include <optional>

namespace midi_command{

    struct QueuedNoteOn{
        units::midi::Channel channel;
        units::midi::Note note;
        units::midi::Velocity velocity;
    };

    struct QueuedNoteOff{
        units::midi::Channel channel;
        units::midi::Note note;
    };

    struct ChangeInstrument{
        units::midi::Channel channel;
        instruments::Subset instrument;
    };

    struct ChangeTempo{
        units::Tempo tempo;
    };

    struct ChangeVolume{
        std::optional<units::midi::Channel> channel;
        units::Volume volume;
    };

    struct ChangeArticulation{
        std::optional<units::midi::Channel> channel;
        units::Articulation articulation;
    };

    using Command = std::variant<
        QueuedNoteOn, 
        QueuedNoteOff,
        ChangeInstrument,
        ChangeTempo,
        ChangeVolume,
        ChangeArticulation
    >;

    void execute(const Command &command, MidiManager &midiManager);

} // namespace midi_command