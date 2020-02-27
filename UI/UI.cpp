//
// Created by petr on 2/22/20.
//

#include "UI.h"
#include <fmt/format.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_sdl.h>

ui::UI::UI(sdl2cpp::Window &window, sdl2cpp::MainLoop &mainLoop, const std::string &glslVersion)
    : ctx(ImGui::CreateContext()), io(ImGui::GetIO()), window(window) {
  IMGUI_CHECKVERSION();
  ImGui::StyleColorsDark();
  ImGui_ImplSDL2_InitForOpenGL(window.getWindow(), window.getContext("rendering"));
  ImGui_ImplOpenGL3_Init(fmt::format("#version {}", glslVersion).c_str());

  panels.emplace_back(&gpuPanel);
  panels.emplace_back(&fpsPanel);
  panels.emplace_back(&renderSettingsPanel);
}

void ui::UI::onFrame() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL2_NewFrame(window.getWindow());
  ImGui::NewFrame();
  for (auto panel : panels) {
    panel->onFrame();
  }
  ImGui::EndFrame();
  render();
}

ui::FPSPanel &ui::UI::getFPSPanel() noexcept { return fpsPanel; }

const ui::FPSPanel &ui::UI::getFPSPanel() const noexcept { return fpsPanel; }

ui::UI::~UI() { ImGui::DestroyContext(ctx); }

void ui::UI::render() {
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
const ui::GPUPanel &ui::UI::getGpuPanel() const { return gpuPanel; }
ui::GPUPanel &ui::UI::getGpuPanel() { return gpuPanel; }

const ui::RenderSettingsPanel &ui::UI::getRenderSettingsPanel() const { return renderSettingsPanel; }

ui::RenderSettingsPanel &ui::UI::getRenderSettingsPanel() { return renderSettingsPanel; }
