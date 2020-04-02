//
// Created by petr on 2/22/20.
//

#ifndef RAYMARCHING_FPSLIMITER_H
#define RAYMARCHING_FPSLIMITER_H

class FPSLimiter {
public:
  explicit FPSLimiter(float limit = 60.f);
  FPSLimiter(FPSLimiter &&other) = delete;
  FPSLimiter(const FPSLimiter &other) = delete;
  auto operator=(FPSLimiter &&other) -> FPSLimiter & = delete;
  auto operator=(const FPSLimiter &other) -> FPSLimiter & = delete;

  // TODO: implement limit -- onFrame(...) function

  auto setEnabled(bool enabled) -> void;
  [[nodiscard]] auto isEnabled() const -> bool;

private:
  bool enabled = false;
  float fpsLimit;
};

#endif // RAYMARCHING_FPSLIMITER_H
