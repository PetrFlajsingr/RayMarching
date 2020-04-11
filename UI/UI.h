//
// Created by petr on 2/22/20.
//

#ifndef RAYMARCHING_UI_H
#define RAYMARCHING_UI_H
#include "CameraPanel.h"
#include "FPSPanel.h"
#include "GPUPanel.h"
#include "MaterialPanel.h"
#include "RenderSettingsPanel.h"
#include <SDL2CPP/Window.h>
#include <imconfig.h>
#include <imgui/imgui.h>

namespace ui {
class UI {
public:
  UI(sdl2cpp::Window &window, sdl2cpp::MainLoop &mainLoop, const std::string &glslVersion, MaterialManager &materialManager);
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

  [[nodiscard]] auto getCameraPanel() const -> const CameraPanel &;
  [[nodiscard]] auto getCameraPanel() -> CameraPanel &;

  [[nodiscard]] auto getMaterialPanel() const -> const MaterialPanel &;
  [[nodiscard]] auto getMaterialPanel() -> MaterialPanel &;

private:
  ImGuiContext *ctx;
  ImGuiIO &io;
  FPSPanel fpsPanel;
  GPUPanel gpuPanel;
  RenderSettingsPanel renderSettingsPanel;
  CameraPanel cameraPanel;
  MaterialPanel materialPanel;

  std::vector<Panel *> panels;
  sdl2cpp::Window &window;

  auto render() -> void;
};
} // namespace ui

#endif // RAYMARCHING_UI_H
