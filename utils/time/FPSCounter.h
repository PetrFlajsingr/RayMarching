//
// Created by Petr on 19.11.2019.
//

#ifndef UTILITIES_FPSCOUNTER_H
#define UTILITIES_FPSCOUNTER_H

#include "now.h"
#include <chrono>
#include <ostream>

using namespace std::chrono_literals;

class FPSCounter {
public:
  [[nodiscard]] float average() const;

  [[nodiscard]] float current() const;

  void frame();

  void restart();

  friend std::ostream &operator<<(std::ostream &stream, const FPSCounter &fpsCounter);

private:
  uint64_t totalFrameCount = 0;
  std::chrono::milliseconds totalTimeSum = 0ms;
  std::chrono::milliseconds lastTimeStamp = now<std::chrono::milliseconds>();
  std::chrono::milliseconds lastFrameDuration = 0ms;
};

#endif // UTILITIES_FPSCOUNTER_H
