//
// Created by petr on 2/12/20.
//
#include "UI/UI.h"
#include "io/print.h"
#include <SDL2CPP/MainLoop.h>
#include <SDL2CPP/Window.h>
#include <SDL_video.h>
#include <error_handling/exceptions.h>
#include <geGL/DebugMessage.h>
#include <geGL/StaticCalls.h>
#include <geGL/geGL.h>
#include <memory>
#include <utility>
#include <magic_enum.hpp>

std::pair<unsigned int, unsigned int> getDisplaySize() {
  SDL_DisplayMode displayMode;
  if (SDL_GetDesktopDisplayMode(0, &displayMode) != 0) {
    throw exc::Error("SDL_GetDesktopDisplayMode failed");
  }
  const auto w = static_cast<unsigned int>(displayMode.w * 0.8);
  const auto h = static_cast<unsigned int>(displayMode.h * 0.8);
  return {w, h};
}
using MainLoop = std::shared_ptr<sdl2cpp::MainLoop>;
using Window = std::shared_ptr<sdl2cpp::Window>;

std::pair<MainLoop, Window> prepareWindow() {
  auto mainLoop = std::make_shared<sdl2cpp::MainLoop>();
  const auto [screenWidth, screenHeight] = getDisplaySize();
  auto window = std::make_shared<sdl2cpp::Window>(screenWidth * 0.8, screenHeight * 0.8);
  window->createContext("rendering", 450);
  mainLoop->addWindow("mainWindow", window);
  return {mainLoop, window};
}

enum class M {
  A, B, C
};

int main() {
  auto mainLoop = std::make_shared<sdl2cpp::MainLoop>();
  const auto [screenWidth, screenHeight] = getDisplaySize();
  auto window = std::make_shared<sdl2cpp::Window>(screenWidth * 0.8, screenHeight * 0.8);
  window->createContext("rendering", 450);
  mainLoop->addWindow("mainWindow", window);

  ge::gl::init(SDL_GL_GetProcAddress);
  ge::gl::setHighDebugMessage();
  ge::gl::glClearColor(0, 0, 0, 1);

  ui::UI ui{*window, *mainLoop, "450"};

  mainLoop->setIdleCallback([&]() {
    ge::gl::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ui.onFrame();
    window->swap();
  });
  (*mainLoop)();
  return 0;
}