#include "music_box.hpp"

void MusicBox::executeNextBufferedCommand(){
	if(commandQueue_.isEmpty()) return;

	auto command{commandQueue_.pop()};
	midi_command::execute(command, midiManager_);
}

