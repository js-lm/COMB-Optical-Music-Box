#pragma once

#include <cstdint>
#include <array>

namespace music_decoder {

    struct InstrumentCommand{
        uint8_t opcode;
        std::array<uint8_t, constants::decoder::NumberOfImmediateDigitsInInstrument> immediateDigits;

        bool isNoOp() const{
            return opcode == 0 
                && immediateDigits[0] == 0 
                && immediateDigits[1] == 0;
        }
    };

    struct SystemCommand{
        uint8_t opcode;
        std::array<uint8_t, constants::decoder::NumberOfImmediateDigitsInCommand> immediateDigits;

        bool isNoOp() const{
            return opcode == 0 
                && immediateDigits[0] == 0 
                && immediateDigits[1] == 0;
        }
    };

    struct DecodedRow{
        std::array<InstrumentCommand, constants::decoder::NumberOfInstrumentChannel> instruments;
        SystemCommand system;
    };

} // namespace music_decoder
