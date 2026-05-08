#pragma once

#include "aliases.hpp"

namespace constants::light_sensor{

    constexpr units::Percentage DefaultThreshold{.5f};
    constexpr units::Percentage MinimumSpread{.1f};

    // constexpr units::Percentage BlackDropAmount{.15f}; 
    // constexpr units::Percentage WhiteRaiseAmount{.05f};

    constexpr units::Us DecayInterval{5000U};
    constexpr units::Percentage DecayAmount{.002f};

    constexpr units::Percentage HysteresisHighPercent{.6f}; 
    constexpr units::Percentage HysteresisLowPercent{.4f};

} // namespace constants::light_sensor




