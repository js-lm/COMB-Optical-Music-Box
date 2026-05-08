#pragma once

#include "aliases.hpp"
#include "constants.hpp"

class LightSensorManager{
private:
    bool isExpectingBlack_{true};
    bool hasArrived_{false};

    units::Percentage recentMaximum_{constants::light_sensor::DefaultThreshold};
    units::Percentage recentMinimum_{constants::light_sensor::DefaultThreshold};
    units::Us lastDecayTime_{0};

public:
    LightSensorManager() = default;
    ~LightSensorManager() = default;

    void initialize();
    void update();

    bool hasArrived() const{ return hasArrived_;}
    void next();

private:
    units::Percentage getLightReading() const;
};
