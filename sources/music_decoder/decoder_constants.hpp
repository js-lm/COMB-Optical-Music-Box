#pragma once

#include <cstdint>

#include "aliases.hpp"

#include "utilities/enum_map.hpp"

namespace constants::decoder{

    constexpr uint8_t NumberOfInstrumentDigit{3};
    constexpr uint8_t NumberOfCommandDigit{3};

    constexpr uint8_t NumberOfImmediateDigitsInInstrument{2};
    constexpr uint8_t NumberOfImmediateDigitsInCommand{2};

    constexpr uint8_t NumberOfInstrumentChannel{4};

    constexpr uint8_t MaximumNumberOfBase5Digit{14};

    constexpr uint8_t Radix{5};

    constexpr uint8_t MaximumSimultaneousNotePerInstrument{16};

    constexpr uint8_t SystemCommandStartIndex{12};

    namespace _{
        using namespace instruments;
    } // namespace _

    constexpr utilities::EnumMap<_::Subset, midi_data::Instrument> InstrumentsMap{
        // Keyboards
        {_::Subset::Grand_Piano,         _::Mt32::Piano_1},
        {_::Subset::Electric_Piano,      _::Mt32::Electric_Piano_1},
        {_::Subset::Drawbar_Organ,       _::Mt32::Organ_1},
        {_::Subset::Church_Organ,        _::Mt32::Church_Organ},

        // Percussion
        {_::Subset::Music_Box,           _::Default::Music_Box},
        {_::Subset::Marimba,             _::Mt32::Marimba},
        {_::Subset::Tubular_Bells,       _::Mt32::Tubular_Bell},
        {_::Subset::Glockenspiel,        _::Mt32::Glockenspiel},
        {_::Subset::Vibraphone,          _::Mt32::Vibraphone_1},
        {_::Subset::Kalimba,             _::Mt32::Kalimba},

        // Woodwinds
        {_::Subset::Flute,               _::Mt32::Flute_1},
        {_::Subset::Oboe,                _::Mt32::Oboe_1},
        {_::Subset::Clarinet,            _::Mt32::Clarinet_1},
        {_::Subset::Bassoon,             _::Mt32::Bassoon},

        // Strings
        {_::Subset::Violin,              _::Mt32::Violin},
        {_::Subset::Viola,               _::Mt32::Viola},
        {_::Subset::Cello,               _::Mt32::Cello_1},
        {_::Subset::Contrabass,          _::Mt32::Contrabass},
        {_::Subset::Pizzicato_Strings,   _::Mt32::Pizzicato_Strings},
        {_::Subset::Orchestral_Harp,     _::Mt32::Harp_1},

        // Brass
        {_::Subset::Trumpet,             _::Mt32::Trumpet},
        {_::Subset::French_Horn,         _::Mt32::French_Horn_1},
        {_::Subset::Trombone,            _::Mt32::Trombone_1},
        {_::Subset::Tuba,                _::Mt32::Tuba},

        // Harmony
        {_::Subset::String_Ensemble,     _::Mt32::Strings},
        {_::Subset::Choir_Aahs,          _::Mt32::Choir_Aahs},

        // Guitars
        {_::Subset::Nylon_Acoustic_Guitar,   _::Mt32::Nylon_String_Guitar},
        {_::Subset::Steel_Acoustic_Guitar,   _::Mt32::Steel_String_Guitar},
        {_::Subset::Electric_Guitar,         _::Mt32::Chorus_Guitar},
        {_::Subset::Overdriven_Guitar,       _::Default::Overdriven_Guitar},

        // Basses
        {_::Subset::Acoustic_Bass,       _::Mt32::Acoustic_Bass},
        {_::Subset::Finger_Bass,         _::Mt32::Fingered_Bass},
        {_::Subset::Fretless_Bass,       _::Mt32::Fretless_Bass_1},
        {_::Subset::Synthesizer_Bass,    _::Mt32::Synth_Bass_1},

        // Reeds
        {_::Subset::Alto_Saxophone,      _::Mt32::Alto_Sax},
        {_::Subset::Tenor_Saxophone,     _::Mt32::Tenor_Sax},

        // Synthesizers
        {_::Subset::Lead_1_Square,       _::Mt32::Square_Wave},
        {_::Subset::Lead_2_Sawtooth,     _::Mt32::Saw_Wave},
        {_::Subset::Lead_3_Calliope,     _::Mt32::Synth_Calliope},
        {_::Subset::Pad_1_Fantasia,      _::Mt32::Fantasia},
        {_::Subset::Pad_2_Warm,          _::Default::Pad_2_Warm},
        {_::Subset::Synthesizer_Strings, _::Mt32::Strings},

        // World
        {_::Subset::Shamisen,            _::Default::Shamisen},
        {_::Subset::Steel_Drum,          _::Default::Steel_Drums},
        {_::Subset::Sitar,               _::Mt32::Sitar},
        {_::Subset::Accordion_French,    _::Mt32::Accordion_French},
        {_::Subset::Harmonica,           _::Mt32::Harmonica},
        {_::Subset::Banjo,               _::Default::Banjo},
        {_::Subset::Pan_Flute,           _::Mt32::Pan_Flute_1},

        // Drum Sets 
        {_::Subset::Drum_Sets, _::Default::Gunshot} // special: channel 10
    };

} // namespace constants::decoder