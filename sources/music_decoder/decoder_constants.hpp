#pragma once

#include <cstdint>

#include "aliases.hpp"

#include "utilities/enum_map.hpp"

namespace constants::decoder{

    constexpr uint8_t NumberOfInstrumentDigit{3};
    constexpr uint8_t NumberOfCommandDigit{3};

    constexpr uint8_t NumberOfInstrumentChannel{4};

    constexpr uint8_t MaximumNumberOfBase5Digit{14};

    constexpr uint8_t Radix{5};

    constexpr uint8_t MaximumSimultaneousNotePerInstrument{5};

    constexpr utilities::EnumMap<instruments::Subset, int> InstrumentsMap{
        // Keyboards
        {instruments::Subset::Grand_Piano, 0},
        {instruments::Subset::Electric_Piano, 4},
        {instruments::Subset::Drawbar_Organ, 16},
        {instruments::Subset::Church_Organ, 19},

        // Percussion
        {instruments::Subset::Music_Box, 10},
        {instruments::Subset::Marimba, 12},
        {instruments::Subset::Tubular_Bells, 14},
        {instruments::Subset::Glockenspiel, 9},
        {instruments::Subset::Vibraphone, 11},
        {instruments::Subset::Kalimba, 108},

        // Woodwinds
        {instruments::Subset::Flute, 73},
        {instruments::Subset::Oboe, 68},
        {instruments::Subset::Clarinet, 71},
        {instruments::Subset::Bassoon, 70},

        // Strings
        {instruments::Subset::Violin, 40},
        {instruments::Subset::Viola, 41},
        {instruments::Subset::Cello, 42},
        {instruments::Subset::Contrabass, 43},
        {instruments::Subset::Pizzicato_Strings, 45},
        {instruments::Subset::Orchestral_Harp, 46},

        // Brass
        {instruments::Subset::Trumpet, 56},
        {instruments::Subset::French_Horn, 60},
        {instruments::Subset::Trombone, 57},
        {instruments::Subset::Tuba, 58},

        // Harmony
        {instruments::Subset::String_Ensemble, 48},
        {instruments::Subset::Choir_Aahs, 52},

        // Guitars
        {instruments::Subset::Nylon_Acoustic_Guitar, 24},
        {instruments::Subset::Steel_Acoustic_Guitar, 25},
        {instruments::Subset::Electric_Guitar, 27},
        {instruments::Subset::Overdriven_Guitar, 29},

        // Basses
        {instruments::Subset::Acoustic_Bass, 32},
        {instruments::Subset::Finger_Bass, 33},
        {instruments::Subset::Fretless_Bass, 35},
        {instruments::Subset::Synthesizer_Bass, 38},

        // Reeds
        {instruments::Subset::Alto_Saxophone, 65},
        {instruments::Subset::Tenor_Saxophone, 66},

        // Synthesizers
        {instruments::Subset::Lead_1_Square, 80},
        {instruments::Subset::Lead_2_Sawtooth, 81},
        {instruments::Subset::Lead_3_Calliope, 82},
        {instruments::Subset::Pad_1_Fantasia, 88},
        {instruments::Subset::Pad_2_Warm, 89},
        {instruments::Subset::Synthesizer_Strings, 50},

        // World
        {instruments::Subset::Shamisen, 106},
        {instruments::Subset::Steel_Drum, 114},
        {instruments::Subset::Sitar, 104},
        {instruments::Subset::Accordion_French, 21},
        {instruments::Subset::Harmonica, 22},
        {instruments::Subset::Banjo, 105},
        {instruments::Subset::Pan_Flute, 75},

        // Drum Sets 
        {instruments::Subset::Drum_Sets, -1} // special: channel 10
    };

} // namespace constants::decoder