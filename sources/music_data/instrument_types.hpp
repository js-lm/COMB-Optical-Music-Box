#pragma once

#include <cstdint>

namespace instruments{

    enum class Subset : uint8_t{
    // Keyboards
        Grand_Piano,
        Electric_Piano,
        Drawbar_Organ,
        Church_Organ,

    // Percussion
        Music_Box,
        Marimba,
        Tubular_Bells,
        Glockenspiel,
        Vibraphone,
        Kalimba,

    // Woodwinds
        Flute,
        Oboe,
        Clarinet,
        Bassoon,

    // Strings
        Violin,
        Viola,
        Cello,
        Contrabass,
        Pizzicato_Strings,
        Orchestral_Harp,

    // Brass
        Trumpet,
        French_Horn,
        Trombone,
        Tuba,

    // Harmony
        String_Ensemble,
        Choir_Aahs,

    // Guitars
        Nylon_Acoustic_Guitar,
        Steel_Acoustic_Guitar,
        Electric_Guitar,
        Overdriven_Guitar,

    // Basses
        Acoustic_Bass,
        Finger_Bass,
        Fretless_Bass,
        Synthesizer_Bass,

    // Reeds
        Alto_Saxophone,
        Tenor_Saxophone,

    // Synthesizers
        Lead_1_Square,
        Lead_2_Sawtooth,
        Lead_3_Calliope,
        Pad_1_Fantasia,
        Pad_2_Warm,
        Synthesizer_Strings,

    // World
        Shamisen,
        Steel_Drum,
        Sitar,
        Accordion_French,
        Harmonica,
        Banjo,
        Pan_Flute,

    // Drum Sets
        Drum_Sets
    };

    enum class Default : uint8_t{
        // piano
        Grand_Piano_1           = 0,
        Bright_Piano_2          = 1,
        Electric_Grand_Piano_3  = 2,
        Honky_Tonk_Piano        = 3,
        Electric_Piano_1        = 4,
        Electric_Piano_2        = 5,
        Harpsichord             = 6,
        Clavi                   = 7,

        // chromatic percussion
        Celesta                 = 8,
        Glockenspiel            = 9,
        Music_Box               = 10,
        Vibraphone              = 11,
        Marimba                 = 12,
        Xylophone               = 13,
        Tubular_Bells           = 14,
        Santur                  = 15,

        // organ
        Drawbar_Organ           = 16,
        Percussive_Organ        = 17,
        Rock_Organ              = 18,
        Church_Organ            = 19,
        Reed_Organ              = 20,
        Accordion_French        = 21,
        Harmonica               = 22,
        Tango_Accordion         = 23,

        // guitar
        Acoustic_Guitar_Nylon   = 24,
        Acoustic_Guitar_Steel   = 25,
        Electric_Guitar_Jazz    = 26,
        Electric_Guitar_Clean   = 27,
        Electric_Guitar_Muted   = 28,
        Overdriven_Guitar       = 29,
        Distortion_Guitar       = 30,
        Guitar_Harmonics        = 31,

        // bass
        Acoustic_Bass           = 32,
        Finger_Bass             = 33,
        Picked_Bass             = 34,
        Fretless_Bass           = 35,
        Slap_Bass_1             = 36,
        Slap_Bass_2             = 37,
        Synthesizer_Bass_1      = 38,
        Synthesizer_Bass_2      = 39,

        // string
        Violin                  = 40,
        Viola                   = 41,
        Cello                   = 42,
        Contrabass              = 43,
        Tremolo_Strings         = 44,
        Pizzicato_Strings       = 45,
        Orchestral_Harp         = 46,
        Timpani                 = 47,

        // ensemble
        String_Ensemble_1       = 48,
        String_Ensemble_2       = 49,
        Synthesizer_Strings_1   = 50,
        Synthesizer_Strings_2   = 51,
        Choir_Aahs              = 52,
        Voice_Oohs              = 53,
        Synthesizer_Voice       = 54,
        Orchestra_Hit           = 55,

        // brass
        Trumpet                 = 56,
        Trombone                = 57,
        Tuba                    = 58,
        Muted_Trumpet           = 59,
        French_Horn             = 60,
        Brass_Section           = 61,
        Synthesizer_Brass_1     = 62,
        Synthesizer_Brass_2     = 63,

        // reed
        Soprano_Sax             = 64,
        Alto_Sax                = 65,
        Tenor_Sax               = 66,
        Baritone_Sax            = 67,
        Oboe                    = 68,
        English_Horn            = 69,
        Bassoon                 = 70,
        Clarinet                = 71,

        // pipe
        Piccolo                 = 72,
        Flute                   = 73,
        Recorder                = 74,
        Pan_Flute               = 75,
        Blown_Bottle            = 76,
        Shakuhachi              = 77,
        Whistle                 = 78,
        Ocarina                 = 79,

        // synth lead
        Lead_1_Square           = 80,
        Lead_2_Sawtooth         = 81,
        Lead_3_Calliope         = 82,
        Lead_4_Chiff            = 83,
        Lead_5_Charang          = 84,
        Lead_6_Voice            = 85,
        Lead_7_Fifths           = 86,
        Lead_8_Bass_Lead        = 87,

        // synth pad
        Pad_1_Fantasia          = 88,
        Pad_2_Warm              = 89,
        Pad_3_Polysynth         = 90,
        Pad_4_Choir             = 91,
        Pad_5_Bowed             = 92,
        Pad_6_Metallic          = 93,
        Pad_7_Halo              = 94,
        Pad_8_Sweep             = 95,

        // synth effects
        Effects_1_Rain          = 96,
        Effects_2_Soundtrack    = 97,
        Effects_3_Crystal       = 98,
        Effects_4_Atmosphere    = 99,
        Effects_5_Brightness    = 100,
        Effects_6_Goblins       = 101,
        Effects_7_Echoes        = 102,
        Effects_8_Sci_Fi        = 103,

        // ethnic
        Sitar                   = 104,
        Banjo                   = 105,
        Shamisen                = 106,
        Koto                    = 107,
        Kalimba                 = 108,
        Bag_Pipe                = 109,
        Fiddle                  = 110,
        Shanai                  = 111,

        // percussive
        Tinkle_Bell             = 112,
        Agogo                   = 113,
        Steel_Drums             = 114,
        Woodblock               = 115,
        Taiko_Drum              = 116,
        Melodic_Tom             = 117,
        Synthesizer_Drum        = 118,
        Reverse_Cymbal          = 119,

        // sound effects
        Guitar_Fret_Noise       = 120,
        Breath_Noise            = 121,
        Seashore                = 122,
        Bird_Tweet              = 123,
        Telephone_Ring          = 124,
        Helicopter              = 125,
        Applause                = 126,
        Gunshot                 = 127
    };

    enum class Mt32 : uint8_t{
        // piano
        Piano_1                 = 0,
        Piano_2                 = 1,
        Piano_3                 = 2,
        Detuned_Electric_Piano_1= 3,
        Electric_Piano_1        = 4,
        Electric_Piano_2        = 5,
        Detuned_Electric_Piano_2= 6,
        Honky_Tonk              = 7,

        // organ
        Organ_1                 = 8,
        Organ_2                 = 9,
        Organ_3                 = 10,
        Detuned_Organ_1         = 11,
        Church_Organ_2          = 12,
        Church_Organ            = 13,
        Church_Organ_3          = 14,
        Accordion_French        = 15,

        // keyboard
        Harpsichord             = 16,
        Coupled_Harpsichord_1   = 17,
        Coupled_Harpsichord_2   = 18,
        Clavinet_1              = 19,
        Clavinet_2              = 20,
        Clavinet_3              = 21,
        Celesta_1               = 22,
        Celesta_2               = 23,

        // synth brass
        Synth_Brass_1           = 24,
        Synth_Brass_2           = 25,
        Synth_Brass_3           = 26,
        Synth_Brass_4           = 27,

        // synth bass
        Synth_Bass_1            = 28,
        Synth_Bass_2            = 29,
        Synth_Bass_3            = 30,
        Synth_Bass_4            = 31,

        // synth pad
        Fantasia                = 32,
        Synth_Calliope          = 33,
        Choir_Aahs              = 34,
        Bowed_Glass             = 35,
        Soundtrack              = 36,
        Atmosphere              = 37,
        Crystal                 = 38,
        Bag_Pipe                = 39,
        Tinkle_Bell_1           = 40,
        Ice_Rain_1              = 41,
        Oboe_1                  = 42,
        Pan_Flute_1             = 43,

        // synth lead
        Saw_Wave                = 44,
        Charang                 = 45,
        Tubular_Bells_1         = 46,
        Square_Wave             = 47,

        // strings
        Strings                 = 48,
        Tremolo_Strings         = 49,
        Slow_Strings            = 50,
        Pizzicato_Strings       = 51,
        Violin                  = 52,
        Viola                   = 53,
        Cello_1                 = 54,
        Cello_2                 = 55,
        Contrabass              = 56,
        Harp_1                  = 57,
        Harp_2                  = 58,

        // guitar
        Nylon_String_Guitar     = 59,
        Steel_String_Guitar     = 60,
        Chorus_Guitar           = 61,
        Funk_Guitar             = 62,
        Sitar                   = 63,

        // bass
        Acoustic_Bass           = 64,
        Fingered_Bass           = 65,
        Picked_Bass             = 66,
        Fretless_Bass_1         = 67,
        Slap_Bass_1             = 68,
        Slap_Bass_2             = 69,
        Fretless_Bass_2         = 70,
        Fretless_Bass_3         = 71,

        // pipe
        Flute_1                 = 72,
        Flute_2                 = 73,
        Piccolo_1               = 74,
        Piccolo_2               = 75,
        Recorder                = 76,
        Pan_Flute_2             = 77,

        // reed
        Soprano_Sax             = 78,
        Alto_Sax                = 79,
        Tenor_Sax               = 80,
        Baritone_Sax            = 81,
        Clarinet_1              = 82,
        Clarinet_2              = 83,
        Oboe_2                  = 84,
        English_Horn            = 85,
        Bassoon                 = 86,
        Harmonica               = 87,

        // brass
        Trumpet                 = 88,
        Muted_Trumpet           = 89,
        Trombone_1              = 90,
        Trombone_2              = 91,
        French_Horn_1           = 92,
        French_Horn_2           = 93,
        Tuba                    = 94,
        Brass_1                 = 95,
        Brass_2                 = 96,

        // chromatic percussion
        Vibraphone_1            = 97,
        Vibraphone_2            = 98,
        Kalimba                 = 99,
        Tinkle_Bell_2           = 100,
        Glockenspiel            = 101,
        Tubular_Bell            = 102,
        Xylophone               = 103,
        Marimba                 = 104,

        // ethnic
        Koto                    = 105,
        Taisho_Koto             = 106,
        Shakuhachi              = 107,
        Whistle_1               = 108,
        Whistle_2               = 109,
        Bottle_Blow             = 110,
        Pan_Flute_3             = 111,

        // percussive
        Timpani                 = 112,
        Melo_Tom_1              = 113,
        Melo_Tom_2              = 114,
        Synth_Drum_1            = 115,
        Synth_Drum_2            = 116,
        Taiko_1                 = 117,
        Taiko_2                 = 118,
        Reverse_Cym             = 119,
        Castanets               = 120,
        Tinkle_Bell_3           = 121,
        Orchestra_Hit           = 122,

        // sound effects
        Telephone               = 123,
        Bird                    = 124,
        Helicopter              = 125,
        Bowed_Glass_2           = 126,
        Ice_Rain_2              = 127
    };
} // namespace instruments