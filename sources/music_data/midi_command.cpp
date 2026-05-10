#include "midi_command.hpp"

#include "midi_unit/midi_manager.hpp"

#include "constants.hpp"

void midi_command::execute(const Command &command, MidiManager &midiManager){
    std::visit([&midiManager](const auto &event){
        using EventType = std::decay_t<decltype(event)>;

        if constexpr(std::is_same_v<EventType, QueuedNoteOn>){
            midiManager.noteOn(event.channel, event.note, event.velocity);
        }else if constexpr(std::is_same_v<EventType, QueuedNoteOff>){
            midiManager.noteOff(event.channel, event.note);
        }else if constexpr(std::is_same_v<EventType, ChangeInstrument>){
            midiManager.setInstrument(
				event.channel, 
				constants::decoder::InstrumentsMap[event.instrument]
			);
        }else if constexpr(std::is_same_v<EventType, ChangeTempo>){
			// tempo is handled by the state machine loop delay, nothing to do here

        }else if constexpr(std::is_same_v<EventType, ChangeVolume>){


        }else if constexpr(std::is_same_v<EventType, ChangeArticulation>){

        }
    }, command);

}