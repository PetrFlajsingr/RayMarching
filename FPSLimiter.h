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
  FPSLimiter &operator=(FPSLimiter &&other) = delete;
  FPSLimiter &operator=(const FPSLimiter &other) = delete;

  // TODO: implement limit -- onFrame(...) function

  void setEnabled(bool enabled);
  [[nodiscard]] bool isEnabled() const;
private:
  bool enabled = false;
  float fpsLimit;
};

#endif // RAYMARCHING_FPSLIMITER_H
