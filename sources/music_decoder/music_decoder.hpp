#pragma once

#include "aliases.hpp"
#include "constants.hpp"

#include "music_data/midi_command.hpp"

#include <array>
#include <memory>
#include <optional>

class MusicDecoder{
public:
    enum class Base5Value : uint8_t{ White = 0, Red = 1, Green = 2, Blue = 3, Black = 4};

    template<size_t NumberOfDigit>
    using Base5 = std::array<Base5Value, NumberOfDigit>;
    using Base10 = int32_t;

    using InstrumentInstruction = Base5<constants::decoder::NumberOfInstrumentDigit>;
    using ChordInstruction = Base5<constants::decoder::NumberOfCommandDigit>;

private:
    // std::array<midi_data::Instrument, constants::decoder::NumberOfInstrumentChannel> instrumentTypes_;

public:
    MusicDecoder() = default;
    ~MusicDecoder() = default;

    std::optional<midi_command::Command> decode(
        const InstrumentInstruction &value,
        const units::midi::Channel channel
    ) const;
    std::optional<midi_command::Command> decode(
        const ChordInstruction &value
    ) const;

private:
    void setInstrument(const midi_data::Instrument &instrument);

    template<size_t NumberOfDigit>
    Base10 base5ToDecimal(const Base5<NumberOfDigit> &value) const{
        static_assert(NumberOfDigit < constants::decoder::MaximumNumberOfBase5Digit);

        Base10 result{0};
        Base10 multiplier{1};
        
        for(uint8_t index{value.size()}; index --> 0;){
            result += static_cast<Base10>(value[index]) * multiplier;
            multiplier *= 5;
        }
        
        return result;
    }
};