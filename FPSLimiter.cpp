//
// Created by petr on 2/22/20.
//

#include "FPSLimiter.h"

FPSLimiter::FPSLimiter(float limit) : fpsLimit(limit) {}
auto FPSLimiter::setEnabled(bool enabled) -> void { FPSLimiter::enabled = enabled; }
auto FPSLimiter::isEnabled() const -> bool { return enabled; }
