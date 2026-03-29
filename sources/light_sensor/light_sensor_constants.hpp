#pragma once

#include "aliases.hpp"

namespace constants::light_sensor{

    constexpr units::Percentage NoPaperThreshold{.9f};
    constexpr units::Percentage WhiteThreshold{.6f}; // above
    constexpr units::Percentage BlackThreshold{.5f}; // below

} // namespace constants::light_sensor
