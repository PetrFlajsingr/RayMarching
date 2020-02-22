//
// Created by petr on 2/22/20.
//

#ifndef RAYMARCHING_UI_H
#define RAYMARCHING_UI_H
#include <imconfig.h>
#include "FPSPanel.h"
#include <SDL2CPP/Window.h>
#include <imgui/imgui.h>

namespace ui {
class UI {
public:
  UI(sdl2cpp::Window &window, sdl2cpp::MainLoop &mainLoop, const std::string &glslVersion);
  ~UI();
  UI(UI &&other) = delete;
  UI(const UI &other) = delete;
  UI &operator=(UI &&other) = delete;
  UI &operator=(const UI &other) = delete;

  void onFrame();

  [[nodiscard]] FPSPanel &getFPSPanel() noexcept;
  [[nodiscard]] const FPSPanel &getFPSPanel() const noexcept;

private:
  ImGuiContext *ctx;
  ImGuiIO &io;
  FPSPanel fpsPanel;
  sdl2cpp::Window &window;

  void render();
};
} // namespace ui

#endif // RAYMARCHING_UI_H
