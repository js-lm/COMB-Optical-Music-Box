#pragma once

#include <cstdint>

#include "aliases.hpp"

namespace constants::motor{

    constexpr bool ForwardDirection{1};
    constexpr units::Us MaxStepInterval{800ULL};
    constexpr units::Us AccelerationStep{20ULL};

    constexpr units::StepsPerSecond DefaultStepRate{4700}; // < 4750

} // namespace constants::motor
