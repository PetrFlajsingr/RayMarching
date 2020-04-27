//
// Created by petr on 2/22/20.
//

#include "UI.h"
#include <fmt/format.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_sdl.h>

ui::UI::UI(sdl2cpp::Window &window, [[maybe_unused]] sdl2cpp::MainLoop &mainLoop, const std::string &glslVersion,
           MaterialManager &materialManager, Camera &camera)
    : ctx(ImGui::CreateContext()), io(ImGui::GetIO()), cameraPanel(camera), materialPanel(materialManager), window(window) {
  IMGUI_CHECKVERSION();
  ImGui::StyleColorsDark();
  ImGui_ImplSDL2_InitForOpenGL(window.getWindow(), window.getContext("rendering"));
  ImGui_ImplOpenGL3_Init(fmt::format("#version {}", glslVersion).c_str());

  panels.emplace_back(&gpuPanel);
  panels.emplace_back(&fpsPanel);
  panels.emplace_back(&renderSettingsPanel);
  panels.emplace_back(&cameraPanel);
  panels.emplace_back(&materialPanel);
}

auto ui::UI::onFrame() -> void {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL2_NewFrame(window.getWindow());
  ImGui::NewFrame();
  for (auto panel : panels) {
    panel->onFrame();
  }
  ImGui::EndFrame();
  render();
}

auto ui::UI::getFPSPanel() noexcept -> ui::FPSPanel & { return fpsPanel; }

auto ui::UI::getFPSPanel() const noexcept -> const ui::FPSPanel & { return fpsPanel; }

ui::UI::~UI() { ImGui::DestroyContext(ctx); }

auto ui::UI::render() -> void {
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
auto ui::UI::getGpuPanel() const -> const ui::GPUPanel & { return gpuPanel; }
auto ui::UI::getGpuPanel() -> ui::GPUPanel & { return gpuPanel; }

auto ui::UI::getRenderSettingsPanel() const -> const ui::RenderSettingsPanel & { return renderSettingsPanel; }

auto ui::UI::getRenderSettingsPanel() -> ui::RenderSettingsPanel & { return renderSettingsPanel; }
auto ui::UI::getCameraPanel() const -> const CameraPanel & { return cameraPanel; }
auto ui::UI::getCameraPanel() -> CameraPanel & { return cameraPanel; }
auto ui::UI::getMaterialPanel() const -> const ui::MaterialPanel & { return materialPanel; }
auto ui::UI::getMaterialPanel() -> ui::MaterialPanel & { return materialPanel; }
