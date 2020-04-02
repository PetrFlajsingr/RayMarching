//
// Created by petr on 2/22/20.
//

#ifndef RAYMARCHING_UI_H
#define RAYMARCHING_UI_H
#include "FPSPanel.h"
#include "GPUPanel.h"
#include "RenderSettingsPanel.h"
#include <SDL2CPP/Window.h>
#include <imconfig.h>
#include <imgui/imgui.h>

namespace ui {
class UI {
public:
  UI(sdl2cpp::Window &window, sdl2cpp::MainLoop &mainLoop, const std::string &glslVersion);
  ~UI();
  UI(UI &&other) = delete;
  UI(const UI &other) = delete;
  auto operator=(UI &&other) -> UI & = delete;
  auto operator=(const UI &other) -> UI & = delete;

  auto onFrame() -> void;

  [[nodiscard]] auto getFPSPanel() noexcept -> FPSPanel &;
  [[nodiscard]] auto getFPSPanel() const noexcept -> const FPSPanel &;

  [[nodiscard]] auto getGpuPanel() const -> const GPUPanel &;
  [[nodiscard]] auto getGpuPanel() -> GPUPanel &;

  [[nodiscard]] auto getRenderSettingsPanel() const -> const RenderSettingsPanel &;
  [[nodiscard]] auto getRenderSettingsPanel() -> RenderSettingsPanel &;

private:
  ImGuiContext *ctx;
  ImGuiIO &io;
  FPSPanel fpsPanel;
  GPUPanel gpuPanel;
  RenderSettingsPanel renderSettingsPanel;

  std::vector<Panel *> panels;
  sdl2cpp::Window &window;

  auto render() -> void;
};
} // namespace ui

#endif // RAYMARCHING_UI_H
