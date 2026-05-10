#include "light_manager.hpp"

#include <hardware/adc.h>

#include <stdio.h>
#include <pico/stdlib.h>

#include "constants.hpp"

#include "debug_utilities.hpp"

void LightSensorManager::initialize(){
    adc_init();

    adc_gpio_init(constants::pins::LightAnalogOut);

    adc_select_input(constants::adc::LightInputChannel);
    
}

void LightSensorManager::update(){
    // /* DEBUG */ {
    //     adc_select_input(constants::adc::LightInputChannel);

    //     DEBUG_PRINT_TIMED(100, "IR raw: %d, getIrReading(): %.3f, hasArrived_: %b\n", 
    //         adc_read(), getLightReading(), hasArrived_
    //     );

    //     DEBUG_PRINT_IF_CHANGED("isExpectingBlack_: %b\n", isExpectingBlack_);
    //     DEBUG_PRINT_IF_CHANGED("hasArrived_: %b\n", hasArrived_);
    // } /* DEBUG */

    const auto lightReading{getLightReading()};
    const units::TimestampUs currentTime{time_us_64()};

    if(lastDecayTime_ == 0){
        lastDecayTime_ = currentTime;
        recentMaximum_ = lightReading;
        recentMinimum_ = lightReading;
    }

    // track edge
    if(lightReading > recentMaximum_) recentMaximum_ = lightReading;
    if(lightReading < recentMinimum_) recentMinimum_ = lightReading;

    // periodically decay the min/max
    if((currentTime - lastDecayTime_) > constants::light_sensor::DecayInterval){ 

        recentMaximum_ -= constants::light_sensor::DecayAmount;
        recentMinimum_ += constants::light_sensor::DecayAmount;

        lastDecayTime_ = currentTime;
    }

    // protect against false triggers
    if((recentMaximum_ - recentMinimum_) < constants::light_sensor::MinimumSpread){
        const units::Percentage midpoint{(recentMaximum_ + recentMinimum_) / 2.0f};
        const units::Percentage halfSpread{constants::light_sensor::MinimumSpread / 2.0f};

        recentMaximum_ = midpoint + halfSpread;
        recentMinimum_ = midpoint - halfSpread;

    }

    // dynamically adapt to new brightness
    const units::Percentage range{recentMaximum_ - recentMinimum_};
    const units::Percentage highThreshold{
        recentMinimum_ + (range * constants::light_sensor::HysteresisHighPercent)
    }; 
    const units::Percentage lowThreshold{
        recentMinimum_ + (range * constants::light_sensor::HysteresisLowPercent)
    }; 

    
    if(isExpectingBlack_){
        if(lightReading < lowThreshold){
            isExpectingBlack_ = false;
        }
    }else{
        if(lightReading > highThreshold){
            hasArrived_ = true;
        }
    }
}
