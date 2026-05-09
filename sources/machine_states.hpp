#pragma once

#include <cstdint>

#include "music_data/instrument_types.hpp"

#include "constants.hpp"

#include "utilities/static_set.hpp"

struct MachineStates{

    enum class State : uint8_t{
        None,

        Seek,
        Sampling,
        Wait,
        Process
    } state{State::None};

    std::array<instruments::Subset, constants::decoder::NumberOfInstrumentChannel> instruments{};
    std::array<units::Volume, constants::decoder::NumberOfInstrumentChannel> volumes{};
    std::array<units::Articulation, constants::decoder::NumberOfInstrumentChannel> articulations{};
    units::Tempo tempo{};

    using ActiveNotes = utilities::StaticSet<midi_data::Note, constants::decoder::MaximumSimultaneousNotePerInstrument>;
    std::array<ActiveNotes, constants::decoder::NumberOfInstrumentChannel> activeNotes{};

};