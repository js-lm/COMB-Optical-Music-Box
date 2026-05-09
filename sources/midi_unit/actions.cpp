#include "midi_manager.hpp"

#include <pico/stdlib.h>
#include <hardware/uart.h>

#include "constants.hpp"

void MidiManager::noteOn(
    units::midi::Channel channel,
    units::midi::Note note,
    units::midi::Velocity velocity
){
    if(channel > constants::midi::MaximumChannel
    || note > constants::midi::MaximumNote
    || velocity > constants::midi::MaximumVelocity
    ){
        return;
    }
    
    uint8_t statusByte{
        static_cast<uint8_t>(
            constants::midi::NoteOnStatus | (channel & constants::midi::ChannelMask)
        )
    };
    
    uart_putc_raw(constants::midi::UartInstance, statusByte);
    uart_putc_raw(constants::midi::UartInstance, note);
    uart_putc_raw(constants::midi::UartInstance, velocity);
}

void MidiManager::noteOff(units::midi::Channel channel, units::midi::Note note){
    if(channel > constants::midi::MaximumChannel || note > constants::midi::MaximumNote){
        return;
    }

    uint8_t statusByte{
        static_cast<uint8_t>(
            constants::midi::NoteOffStatus | (channel & constants::midi::ChannelMask)
        )
    };

    uart_putc_raw(constants::midi::UartInstance, statusByte);
    uart_putc_raw(constants::midi::UartInstance, note);
    uart_putc_raw(constants::midi::UartInstance, 0);
}

void MidiManager::silence(units::midi::Channel channel){
    if(channel > constants::midi::MaximumChannel) return;

    uint8_t statusByte{
        static_cast<uint8_t>(
            constants::midi::ControlChangeStatus | (channel & constants::midi::ChannelMask)
        )
    };
    
    uart_putc_raw(constants::midi::UartInstance, statusByte);
    uart_putc_raw(constants::midi::UartInstance, constants::midi::AllSoundOffController); 
    uart_putc_raw(constants::midi::UartInstance, constants::midi::ControllerValueOff);
}