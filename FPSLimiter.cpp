//
// Created by petr on 2/22/20.
//

#include "FPSLimiter.h"

FPSLimiter::FPSLimiter(float limit) : fpsLimit(limit) {}
void FPSLimiter::setEnabled(bool enabled) {
  FPSLimiter::enabled = enabled;
}
bool FPSLimiter::isEnabled() const {
  return enabled;
}
