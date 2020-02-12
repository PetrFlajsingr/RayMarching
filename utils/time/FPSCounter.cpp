//
// Created by Petr on 19.11.2019.
//

#include "FPSCounter.h"

float FPSCounter::average() const {
  return static_cast<float>(totalFrameCount) / totalTimeSum.count() *
         std::chrono::duration_cast<std::chrono::nanoseconds>(1s).count();
}

float FPSCounter::current() const {
  return 1.f / lastFrameDuration.count() * std::chrono::duration_cast<std::chrono::nanoseconds>(1s).count();
}

void FPSCounter::frame() {
  ++totalFrameCount;
  const auto timeStamp = now<std::chrono::milliseconds>();
  lastFrameDuration = timeStamp - lastTimeStamp;
  totalTimeSum += lastFrameDuration;
  lastTimeStamp = timeStamp;
}

void FPSCounter::restart() {
  totalFrameCount = 0;
  totalTimeSum = 0ms;
  lastFrameDuration = 0ms;
  lastTimeStamp = now<std::chrono::milliseconds>();
}

std::ostream &operator<<(std::ostream &stream, const FPSCounter &fpsCounter) {
  stream << "[FPS]: average: " << fpsCounter.average() << " immediate: " << fpsCounter.current();
  return stream;
}