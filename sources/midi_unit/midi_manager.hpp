#pragma once

#include <cstdint>
#include <span>

#include "aliases.hpp"

class MidiManager{
public:
    MidiManager() = default;
    ~MidiManager() = default;

    void initialization();

public:
    void noteOn(units::midi::Channel channel, units::midi::Note note, units::midi::Velocity velocity);
    void noteOff(units::midi::Channel channel, units::midi::Note note);

    void silence(units::midi::Channel channel);
    
public:
    void setInstrument(units::midi::Channel channel, const midi_data::Instrument &instrument);

};