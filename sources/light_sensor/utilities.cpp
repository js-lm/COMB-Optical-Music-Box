#include "light_manager.hpp"

#include "constants.hpp"

#include <hardware/adc.h>

units::Percentage LightSensorManager::getLightReading() const{
    adc_select_input(constants::adc::LightInputChannel); 
    
    return static_cast<float>(adc_read()) / constants::system::MaximumAdcValue;
}

void LightSensorManager::next(){
    isExpectingBlack_ = !isExpectingBlack_;
    hasArrived_ = false;
}