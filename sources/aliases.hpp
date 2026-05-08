#pragma once

#include <cstdint>
#include <memory>
#include <variant>

#include "music_data/instrument_types.hpp"

namespace units{

    using Ms = uint32_t;
    using Us = uint64_t;

    inline constexpr Us Ms2Us(Ms millisecond){ return static_cast<Us>(millisecond) * 1000;}
    inline constexpr Ms Us2Ms(Us microsecond){ return static_cast<Ms>(microsecond / 1000);}

    using MsRate = uint16_t;
    using UsRate = uint16_t;
    
    using TimestampMs = uint64_t;
    using TimestampUs = uint64_t;

    using Percentage = float;
    using StepsPerSecond = uint16_t;

    namespace midi{

        using Channel   = uint8_t;
        using Note      = uint8_t;
        using NoteOffset= int8_t;
        using Velocity  = uint8_t;
        using Octave    = int8_t;

    } // namespace midi

    // using InstrumentChannel = uint8_t;
    using Tempo = uint8_t;
    using Volume = uint8_t;

    enum class Articulation : uint8_t{
        Staccato,
        Normal,
        Legato,
        Sustain,
        Infinite
    };

} // namespace units

namespace midi_data{

    using Instrument = std::variant<instruments::Default, instruments::Mt32>;

    // using UniqueEventPointer = std::unique_ptr<midi_command::BaseCommand>;

    enum class Note : uint8_t{
        C0 = 0,
        C_Sharp_0,
        D_Flat_0 = C_Sharp_0,
        D0,
        D_Sharp_0,
        E_Flat_0 = D_Sharp_0,
        E0,
        F0,
        F_Sharp_0,
        G_Flat_0 = F_Sharp_0,
        G0,
        G_Sharp_0,
        A_Flat_0 = G_Sharp_0,
        A0,
        A_Sharp_0,
        B_Flat_0 = A_Sharp_0,
        B0,

        C1 = 12,
        C_Sharp_1,
        D_Flat_1 = C_Sharp_1,
        D1,
        D_Sharp_1,
        E_Flat_1 = D_Sharp_1,
        E1,
        F1,
        F_Sharp_1,
        G_Flat_1 = F_Sharp_1,
        G1,
        G_Sharp_1,
        A_Flat_1 = G_Sharp_1,
        A1,
        A_Sharp_1,
        B_Flat_1 = A_Sharp_1,
        B1,

        C2 = 24,
        C_Sharp_2,
        D_Flat_2 = C_Sharp_2,
        D2,
        D_Sharp_2,
        E_Flat_2 = D_Sharp_2,
        E2,
        F2,
        F_Sharp_2,
        G_Flat_2 = F_Sharp_2,
        G2,
        G_Sharp_2,
        A_Flat_2 = G_Sharp_2,
        A2,
        A_Sharp_2,
        B_Flat_2 = A_Sharp_2,
        B2,

        C3 = 36,
        C_Sharp_3,
        D_Flat_3 = C_Sharp_3,
        D3,
        D_Sharp_3,
        E_Flat_3 = D_Sharp_3,
        E3,
        F3,
        F_Sharp_3,
        G_Flat_3 = F_Sharp_3,
        G3,
        G_Sharp_3,
        A_Flat_3 = G_Sharp_3,
        A3,
        A_Sharp_3,
        B_Flat_3 = A_Sharp_3,
        B3,

        C4 = 48,
        C_Sharp_4,
        D_Flat_4 = C_Sharp_4,
        D4,
        D_Sharp_4,
        E_Flat_4 = D_Sharp_4,
        E4,
        F4,
        F_Sharp_4,
        G_Flat_4 = F_Sharp_4,
        G4,
        G_Sharp_4,
        A_Flat_4 = G_Sharp_4,
        A4,
        A_Sharp_4,
        B_Flat_4 = A_Sharp_4,
        B4,

        C5 = 60,
        C_Sharp_5,
        D_Flat_5 = C_Sharp_5,
        D5,
        D_Sharp_5,
        E_Flat_5 = D_Sharp_5,
        E5,
        F5,
        F_Sharp_5,
        G_Flat_5 = F_Sharp_5,
        G5,
        G_Sharp_5,
        A_Flat_5 = G_Sharp_5,
        A5,
        A_Sharp_5,
        B_Flat_5 = A_Sharp_5,
        B5,

        C6 = 72,
        C_Sharp_6,
        D_Flat_6 = C_Sharp_6,
        D6,
        D_Sharp_6,
        E_Flat_6 = D_Sharp_6,
        E6,
        F6,
        F_Sharp_6,
        G_Flat_6 = F_Sharp_6,
        G6,
        G_Sharp_6,
        A_Flat_6 = G_Sharp_6,
        A6,
        A_Sharp_6,
        B_Flat_6 = A_Sharp_6,
        B6,

        C7 = 84,
        C_Sharp_7,
        D_Flat_7 = C_Sharp_7,
        D7,
        D_Sharp_7,
        E_Flat_7 = D_Sharp_7,
        E7,
        F7,
        F_Sharp_7,
        G_Flat_7 = F_Sharp_7,
        G7,
        G_Sharp_7,
        A_Flat_7 = G_Sharp_7,
        A7,
        A_Sharp_7,
        B_Flat_7 = A_Sharp_7,
        B7,

        C8 = 96,
        C_Sharp_8,
        D_Flat_8 = C_Sharp_8,
        D8,
        D_Sharp_8,
        E_Flat_8 = D_Sharp_8,
        E8,
        F8,
        F_Sharp_8,
        G_Flat_8 = F_Sharp_8,
        G8,
        G_Sharp_8,
        A_Flat_8 = G_Sharp_8,
        A8,
        A_Sharp_8,
        B_Flat_8 = A_Sharp_8,
        B8
    };

} // namespace midi_data

namespace physical{

    using Pin           = uint8_t;
    using I2CAddress    = uint8_t;
    using Register      = uint8_t;
    using Channel       = uint8_t;

} // namespace physical

namespace color_sensor_data{

    using SensorIndex = uint8_t;

    struct RawColorReading{
        uint16_t red;
        uint16_t green;
        uint16_t blue;
        uint16_t clear;
    };

} // namespace color_sensor_data

