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
            states_.articulations.fill(articulation);
        }else if(target <= 4){
            states_.articulations[target - 1] = articulation;
        }
    } break;
    }
}

void MusicStateMachine::evaluateInstrumentCommands(
    const InstrumentCommands &instrumentCommands, 
    CommandBuffer &queue
){
    for(uint8_t channelIndex{0}; channelIndex < constants::decoder::NumberOfInstrumentChannel; channelIndex++){
        const auto &command{instrumentCommands[channelIndex]};
        const auto articulation{states_.articulations[channelIndex]};
        
        std::optional<midi_data::Note> currentNote{std::nullopt};

        if(!command.isNoOp()){
            if(command.opcode <= 2){

                currentNote = static_cast<midi_data::Note>(decodeBase5(
                    command.opcode,
                    command.immediateDigits[0],
                    command.immediateDigits[1]
                ) + constants::system::BaseNoteOffset);

            }else if(command.opcode == 3 || command.opcode == 4){
                const auto instrumentId{decodeBase5(command.opcode - 3, command.immediateDigits[0], command.immediateDigits[1])};
                
                states_.instruments[channelIndex] = static_cast<instruments::Subset>(instrumentId);

                queue.push(midi_command::ChangeInstrument{
                    .channel = channelIndex, 
                    .instrument = states_.instruments[channelIndex]
                });
            }
        }

        const auto targetChannel{
            states_.instruments[channelIndex] == instruments::Subset::Drum_Sets 
                ? constants::midi::DrumSetsChannel : channelIndex
        };

        if(currentNote.has_value()){
            const midi_data::Note note{currentNote.value()};
            const auto noteValue{static_cast<units::midi::Note>(note)};

            switch(articulation){
            case units::Articulation::Infinite: {

                if(states_.activeNotes[channelIndex].remove(note)){
                    states_.activeNotes[channelIndex].add(note);
                    queue.push(midi_command::QueuedNoteOff{
                        .channel = targetChannel, 
                        .note = noteValue
                    });
                }else{
                    queue.push(midi_command::QueuedNoteOn{
                        .channel = targetChannel, 
                        .note = noteValue, 
                        .velocity = static_cast<units::midi::Velocity>((states_.volumes[channelIndex] * constants::midi::MaximumVelocity) / 100.0f)
                    });
                }
                
                states_.activeNotes[channelIndex].add(note);
            } break;
            
            case units::Articulation::Sustain:
            case units::Articulation::Legato: {

                if(states_.activeNotes[channelIndex].remove(note)){
                    states_.activeNotes[channelIndex].add(note);
                }else{
                    const auto activeNotes{states_.activeNotes[channelIndex].toVector()};
                    for(const auto &activeNote : activeNotes){
                        queue.push(midi_command::QueuedNoteOff{
                            .channel = targetChannel, 
                            .note = static_cast<units::midi::Note>(activeNote)
                        });
                    }

                    states_.activeNotes[channelIndex].clear();
                    states_.activeNotes[channelIndex].add(note);

                    queue.push(midi_command::QueuedNoteOn{
                        .channel = targetChannel, 
                        .note = noteValue, 
                        .velocity = static_cast<units::midi::Velocity>((states_.volumes[channelIndex] * constants::midi::MaximumVelocity) / 100.0f)
                    });
                }
            } break;

            case units::Articulation::Normal:
            case units::Articulation::Staccato:
            default: {
                const auto activeNotes{states_.activeNotes[channelIndex].toVector()};
                for(const auto &activeNote : activeNotes){
                    queue.push(midi_command::QueuedNoteOff{
                        .channel = targetChannel, 
                        .note = static_cast<units::midi::Note>(activeNote)
                    });
                }
                queue.push(midi_command::QueuedNoteOn{ // TODO: all duplicated, make a factory
                    .channel = targetChannel, 
                    .note = noteValue, 
                    .velocity = static_cast<units::midi::Velocity>((states_.volumes[channelIndex] * constants::midi::MaximumVelocity) / 100.0f)
                });
                states_.activeNotes[channelIndex].clear();
                states_.activeNotes[channelIndex].add(note);

            } break;
            }
        }else{
            switch(articulation){
            case units::Articulation::Staccato:
                // TODO: staccato


            case units::Articulation::Normal:
            case units::Articulation::Legato: {
                // const auto activeNotes{context_.machine.activeNotes[channel].toVector()};
                // for(const auto &activeNote : activeNotes){
                //     midiManager.noteOff(targetChannel, activeNote);
                // }

                const auto activeNotes{states_.activeNotes[channelIndex].toVector()};
                for(const auto &activeNote : activeNotes){
                    queue.push(midi_command::QueuedNoteOff{
                        .channel = targetChannel, 
                        .note = static_cast<units::midi::Note>(activeNote)
                    });
                }
                states_.activeNotes[channelIndex].clear();
            } break;

            case units::Articulation::Sustain:
            case units::Articulation::Infinite:
            default: break;
            }
        }
    
    }
}
