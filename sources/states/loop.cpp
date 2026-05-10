#include "music_box.hpp"

#include "debug_utilities.hpp"

#include <optional>

void MusicBox::updateSeekState(){
    lightSensorManager_.update();

    if(lightSensorManager_.hasArrived()){
        motorManager_.stop();
        motorStopTimestamp_ = time_us_64();

        nextState();
    }
}

void MusicBox::updateSamplingState(){
    const units::TimestampUs currentTime{time_us_64()};
    if(currentTime - motorStopTimestamp_ >= constants::color_sensor::SeekStateStopToSamplingSettleDelay){
        sensorsManager_.startSampling();
        nextState();
    }
}

void MusicBox::updateWaitState(){
    const units::Percentage tempoPercentage{musicStateMachine_.getStates().tempo / 100.0f};
    const float targetNps{constants::system::MaximumNotesPerSecond * tempoPercentage}; // TODO: cache this
    
    const units::Us targetInterval{static_cast<units::Us>(1000000.0f / targetNps)}; // TODO: helper function
    const bool isTempoReady{(time_us_64() - lastNoteTimestamp_) >= targetInterval};

    // DEBUG_PRINT("targetInterval:%d, isTempoReady: %d", 
    //     targetInterval, isTempoReady
    // );

    if(sensorsManager_.isSamplingReady() && isTempoReady){
        lastNoteTimestamp_ = time_us_64();

        while(!commandQueue_.isEmpty()){
            executeNextBufferedCommand();
        }

        nextState();
    }
}

void MusicBox::updateProcessState(){
    const auto colorRow{sensorsManager_.collectSensorData()};

    // constexpr auto numberOfInstrumentCount{constants::decoder::NumberOfInstrumentChannel};

    music_decoder::DecodedRow decodedRow{};

    auto getDigit{[&](int index)->uint8_t{
        return static_cast<uint8_t>(
            colorRow[constants::color_sensor::SensorIndexMap[index]]
        );
    }};

    // 0 - 11
    for(uint8_t channelIndex{0}; channelIndex < constants::decoder::NumberOfInstrumentChannel; channelIndex++){
        int startIndex{channelIndex * constants::decoder::NumberOfInstrumentDigit};
        decodedRow.instruments[channelIndex].opcode = getDigit(startIndex);
        decodedRow.instruments[channelIndex].immediateDigits[0] = getDigit(startIndex + 1);
        decodedRow.instruments[channelIndex].immediateDigits[1] = getDigit(startIndex + 2);
    }

    // 12 - 14
    decodedRow.system.opcode = getDigit(constants::decoder::SystemCommandStartIndex);
    decodedRow.system.immediateDigits[0] = getDigit(constants::decoder::SystemCommandStartIndex + 1);
    decodedRow.system.immediateDigits[1] = getDigit(constants::decoder::SystemCommandStartIndex + 2);

    musicStateMachine_.process(decodedRow, commandQueue_);

    const auto &states{musicStateMachine_.getStates()};
    DEBUG_PRINT(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>.>>>>>>>");
    DEBUG_PRINT("Tempo: %d", states.tempo);
    for(uint8_t channelindex{0}; channelindex < constants::decoder::NumberOfInstrumentChannel; channelindex++){
        const auto articulationName{magic_enum::enum_name(states.articulations[channelindex])};
        DEBUG_PRINT("Channel %d - V:%d, A:%.*s", 
            channelindex,
            states.volumes[channelindex],
            static_cast<int>( articulationName.size()),
            articulationName.data()
        );
        
        const auto activeNotes{states.activeNotes[channelindex].toVector()};
        for(const auto &note : activeNotes){
            const auto noteName{magic_enum::enum_name(note)};
            DEBUG_PRINT(" -> Playing note: %d (%.*s)", 
                static_cast<int>(note), 
                static_cast<int>(noteName.size()),
                noteName.data()
            );
        }
    }

    sensorsManager_.stopSampling();
    lightSensorManager_.next();
    motorManager_.start();

    nextState();
}

void MusicBox::DEBUG_bitFlipTester(){
    using namespace constants::color_sensor;

    constexpr units::Ms spinDuration{100};
    constexpr units::Us settleDurationUs{SeekStateStopToSamplingSettleDelay};

    while(true){
        motorManager_.start();
        DEBUG_SLEEP_MS(spinDuration);

        motorManager_.stop();
        sleep_us(settleDurationUs);

        sensorsManager_.startSampling();
        while(!sensorsManager_.isSamplingReady()){
            tight_loop_contents();
        }

        const auto colorRawRow{sensorsManager_.DEBUG_collectSensorRawReadings()};

        for(int sensorIndex{0}; sensorIndex < TotalSensorCount; sensorIndex++){
            DEBUG_PRINT("[%i:%i] r:%i g:%i b:%i c:%i",
                sensorIndex,
                SensorIndexMap[sensorIndex],
                static_cast<int>(colorRawRow[sensorIndex].red),
                static_cast<int>(colorRawRow[sensorIndex].green),
                static_cast<int>(colorRawRow[sensorIndex].blue),
                static_cast<int>(colorRawRow[sensorIndex].clear)
            );
        }

        sensorsManager_.stopSampling();
    }
}

void MusicBox::DEBUG_updateProcessState(){
    // DEBUG_bitFlipTester();

    const auto completeColorDataRow{sensorsManager_.DEBUG_getCompleteColorDataRow()};

    DEBUG_PRINT(
        "checksum valid:%i residue:%i correction:%i",
        static_cast<int>(completeColorDataRow.isChecksumValid),
        static_cast<int>(completeColorDataRow.checksumResidue),
        static_cast<int>(completeColorDataRow.correctionApplied)
    );

    if(completeColorDataRow.correctionApplied){
        DEBUG_PRINT(
            "corrected sensor:%i from:%i(cost:%i) to:%i(cost:%i)",
            static_cast<int>(completeColorDataRow.correctedSensorIndex),
            static_cast<int>(completeColorDataRow.correctedFromColor),
            static_cast<int>(completeColorDataRow.correctedFromCost),
            static_cast<int>(completeColorDataRow.correctedToColor),
            static_cast<int>(completeColorDataRow.correctedToCost)
        );
    }

    using namespace constants::color_sensor;
    for(uint8_t sensorPrintIndex{0}; sensorPrintIndex < TotalSensorCount; sensorPrintIndex++){
        const auto mappedSensorIndex{SensorIndexMap[sensorPrintIndex]};

        const auto sensorData{completeColorDataRow.sensorDataRow[mappedSensorIndex]};
        const auto finalColor{completeColorDataRow.finalColorRow[mappedSensorIndex]};

        DEBUG_PRINT(
            "sensor:%i detected:%i final:%i r:%i g:%i b:%i c:%i",
            static_cast<int>(sensorPrintIndex),
            static_cast<int>(sensorData.detectedColor),
            static_cast<int>(finalColor),
            static_cast<int>(sensorData.rawColorReading.red),
            static_cast<int>(sensorData.rawColorReading.green),
            static_cast<int>(sensorData.rawColorReading.blue),
            static_cast<int>(sensorData.rawColorReading.clear)
        );
    }

    sensorsManager_.stopSampling();
    lightSensorManager_.next();
    motorManager_.start();

    nextState();
}