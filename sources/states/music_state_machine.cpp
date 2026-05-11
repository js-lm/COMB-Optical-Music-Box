#include "states/music_state_machine.hpp"

void MusicStateMachine::process(
    const music_decoder::DecodedRow &row, 
    CommandBuffer &queue
){
    if(!row.system.isNoOp()) updateSystemState(row.system, queue);
    
    evaluateInstrumentCommands(row.instruments, queue);
}

void MusicStateMachine::updateSystemState(
    const music_decoder::SystemCommand &systemCommand, 
    CommandBuffer &queue
){
    uint8_t opcode{systemCommand.opcode};
    uint8_t immediate1{systemCommand.immediateDigits[0]};
    uint8_t immediate2{systemCommand.immediateDigits[1]};

    switch(opcode){
    case 0:
    case 1: {
        states_.tempo = (decodeBase5(opcode, immediate1, immediate2) + 1) * 2;
    } break;

    case 2:
    case 3: {
        uint8_t target{immediate1};

        const auto volume{static_cast<units::Volume>((((opcode - 2) * 5) + immediate2 + 1) * 10)};
        
        if(target == 0){
            states_.volumes.fill(volume);
            // queue.push(midi_command::ChangeVolume{.channel = std::nullopt, .volume = volume});
        }else if(target <= 4){
            states_.volumes[target - 1] = volume;
            // queue.push(midi_command::ChangeVolume{
            //     .channel = static_cast<units::midi::Channel>(target - 1), 
            //     .volume = volume
            // });
        }
    } break;

    case 4: {
        uint8_t target{immediate1};
        const auto articulation{static_cast<units::Articulation>(immediate2)};

        if(target == 0){
            for(uint8_t instrumentChannel{0}; instrumentChannel < constants::decoder::NumberOfInstrumentChannel; instrumentChannel++){
                applyArticulation(instrumentChannel, articulation, queue);
            }
        }else if(target <= 4){
            applyArticulation(target - 1, articulation, queue);
        }
    } break;
    }
}

void MusicStateMachine::evaluateInstrumentCommands(
    const InstrumentCommands &instrumentCommands, 
    CommandBuffer &queue
){
    for(uint8_t instrumentChannel{0}; instrumentChannel < constants::decoder::NumberOfInstrumentChannel; instrumentChannel++){
        const auto &command{instrumentCommands[instrumentChannel]};
        const auto articulation{states_.articulations[instrumentChannel]};
        
        std::optional<midi_data::Note> currentNote{std::nullopt};

        bool instrumentChanged{false};

        const auto targetChannel{
            states_.instruments[instrumentChannel] == instruments::Subset::Drum_Sets 
                ? constants::midi::DrumSetsChannel : instrumentChannel
        };
        
        if(!command.isNoOp()){
            if(command.opcode <= 2){

                currentNote = static_cast<midi_data::Note>(decodeBase5(
                    command.opcode,
                    command.immediateDigits[0],
                    command.immediateDigits[1]
                ) + constants::system::BaseNoteOffset);

            }else if(command.opcode == 3 || command.opcode == 4){
                const auto instrumentId{decodeBase5(command.opcode - 3, command.immediateDigits[0], command.immediateDigits[1])};
                
                states_.instruments[instrumentChannel] = static_cast<instruments::Subset>(instrumentId);

                instrumentChanged = true;

                // queue.push(midi_command::ChangeInstrument{
                //     .channel = instrumentChannel, 
                //     .instrument = states_.instruments[instrumentChannel]
                // });
            }
        }

        if(currentNote.has_value()){
            const midi_data::Note note{currentNote.value()};
            const auto noteValue{static_cast<units::midi::Note>(note)};

            switch(articulation){
            case units::Articulation::Infinite: {

                if(states_.activeNotes[instrumentChannel].remove(note)){
                    states_.activeNotes[instrumentChannel].add(note);
                    queueNoteOff(targetChannel, noteValue, queue);
                }else{
                    queueNoteOn(instrumentChannel, targetChannel, noteValue, queue);
                }
                
                states_.activeNotes[instrumentChannel].add(note);
            } break;
            
            case units::Articulation::Sustain:
            case units::Articulation::Legato: {

                if(states_.activeNotes[instrumentChannel].remove(note)){
                    states_.activeNotes[instrumentChannel].add(note);
                }else{
                    clearActiveNotes(instrumentChannel, targetChannel, queue);
                    states_.activeNotes[instrumentChannel].add(note);

                    queueNoteOn(instrumentChannel, targetChannel, noteValue, queue);
                }
            } break;

            case units::Articulation::Normal:
            case units::Articulation::Staccato:
            default: {
                clearActiveNotes(instrumentChannel, targetChannel, queue);
                queueNoteOn(instrumentChannel, targetChannel, noteValue, queue);
                states_.activeNotes[instrumentChannel].add(note);

            } break;
            }
        }else{
            switch(articulation){
            case units::Articulation::Staccato:
                // TODO: staccato

            case units::Articulation::Normal:
            case units::Articulation::Legato: {
                 clearActiveNotes(instrumentChannel, targetChannel, queue);
            } break;

            case units::Articulation::Sustain:
            case units::Articulation::Infinite:
            default: break;
            }
        }

        if(instrumentChanged){
            queue.push(midi_command::ChangeInstrument{
                .channel = instrumentChannel, 
                .instrument = states_.instruments[instrumentChannel]
            });
        }
    
    }
}
