//
// Created by petr on 2/12/20.
//
#include "UI/UI.h"
#include <SDL2CPP/MainLoop.h>
#include <SDL2CPP/Window.h>
#include <SDL_video.h>
#include <error_handling/exceptions.h>
#include <geGL/DebugMessage.h>
#include <geGL/StaticCalls.h>
#include <geGL/geGL.h>
#include <memory>
#include <utility>
#include <spdlog/spdlog.h>
#include "ray_marching/RayMarcher.h"
#include "common/GlslShaderLoader.h"
std::pair<unsigned int, unsigned int> getDisplaySize() {
  SDL_DisplayMode displayMode;
  if (SDL_GetDesktopDisplayMode(0, &displayMode) != 0) {
    throw exc::Error("SDL_GetDesktopDisplayMode failed");
  }
  const auto w = static_cast<unsigned int>(displayMode.w * 0.8);
  const auto h = static_cast<unsigned int>(displayMode.h * 0.8);
  return {w, h};
}

int main() {
  auto mainLoop = std::make_shared<sdl2cpp::MainLoop>();
  const auto [screenWidth, screenHeight] = getDisplaySize();
  spdlog::info("Window size: {}x{}", static_cast<int>(screenWidth * 0.8), static_cast<int>(screenHeight * 0.8));
  auto window = std::make_shared<sdl2cpp::Window>(screenWidth * 0.8, screenHeight * 0.8);
  spdlog::info("OpenGL context version {}", 450);
  window->createContext("rendering", 450);
  mainLoop->addWindow("mainWindow", window);

  ge::gl::init(SDL_GL_GetProcAddress);
  ge::gl::setHighDebugMessage();
  ge::gl::glClearColor(0, 0, 0, 1);

  ui::UI ui{*window, *mainLoop, "450"};

  setShaderLocation("/home/petr/CLionProjects/RayMarching");
  ray_march::RayMarcher rayMarcher{{screenWidth, screenHeight}};

  mainLoop->setIdleCallback([&]() {
    ge::gl::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    rayMarcher.render();
    rayMarcher.show(static_cast<ray_march::Tex>(static_cast<int>(ui.getRenderSettingsPanel().getSelectedTextureType())));
    ui.onFrame();
    window->swap();
  });
  spdlog::info("Starting main loop");
  (*mainLoop)();
  return 0;
}