//
// Created by petr on 2/12/20.
//
#include "UI/UI.h"
#include "common/GlslShaderLoader.h"
#include "ray_marching/RayMarcher.h"
#include <Camera.h>
#include <SDL2CPP/MainLoop.h>
#include <SDL2CPP/Window.h>
#include <SDL_video.h>
#include <error_handling/exceptions.h>
#include <geGL/DebugMessage.h>
#include <geGL/StaticCalls.h>
#include <geGL/geGL.h>
#include <memory>
#include <spdlog/spdlog.h>
#include <utility>
auto getDisplaySize() -> std::pair<unsigned int, unsigned int> {
  SDL_DisplayMode displayMode;
  if (SDL_GetDesktopDisplayMode(0, &displayMode) != 0) {
    throw exc::Error("SDL_GetDesktopDisplayMode failed");
  }
  const auto w = static_cast<unsigned int>(displayMode.w * 0.8);
  const auto h = static_cast<unsigned int>(displayMode.h * 0.8);
  return {w, h};
}

auto main() -> int {
  spdlog::set_level(spdlog::level::debug);
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

  setShaderLocation("/home/petr/CLionProjects/RayMarching/ray_marching");
  ray_march::RayMarcher rayMarcher{{screenWidth, screenHeight}};

  float time = 0;

  Camera camera{PerspectiveProjection{0, 0, 0, 0}};
  bool isCameraControlled = false;
  window->setEventCallback(SDL_MOUSEMOTION, [&isCameraControlled, &camera] (const SDL_Event &event) {
    if (isCameraControlled) {
      camera.ProcessMouseMovement(event.motion.xrel, event.motion.yrel);
      return true;
    }
    return false;
  });
  window->setEventCallback(SDL_MOUSEBUTTONDOWN, [&isCameraControlled] (const SDL_Event &event) {
    isCameraControlled = event.button.button == SDL_BUTTON_RIGHT;
    return isCameraControlled;
  });
  window->setEventCallback(SDL_MOUSEBUTTONUP, [&isCameraControlled] (const SDL_Event &event) {
    if (isCameraControlled && event.button.button == SDL_BUTTON_RIGHT) {
      isCameraControlled = false;
      return true;
    }
    return false;
  });
  window->setEventCallback(SDL_KEYDOWN, [&isCameraControlled, &camera] (const SDL_Event &event) {
    if (isCameraControlled) {
      const auto pressedKey = event.key.keysym.sym;
      switch (pressedKey) {
      case SDLK_w:
        camera.ProcessKeyboard(Camera_Movement::FORWARD, 0.1f);
        break;
      case SDLK_a:
        camera.ProcessKeyboard(Camera_Movement::RIGHT, 0.1f);
        break;
      case SDLK_s:
        camera.ProcessKeyboard(Camera_Movement::BACKWARD, 0.1f);
        break;
      case SDLK_d:
        camera.ProcessKeyboard(Camera_Movement::LEFT, 0.1f);
        break;
      }

      return true;
    }
    return false;
  });

  mainLoop->setIdleCallback([&]() {
    ge::gl::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    rayMarcher.setRayStepLimit(ui.getRenderSettingsPanel().getRayStepLimit());
    rayMarcher.setMaxDrawDistance(ui.getRenderSettingsPanel().getMaxDrawDistance());
    rayMarcher.setTime(time);
    rayMarcher.setCameraVec(camera.Position, camera.Front);
    rayMarcher.setAmbientOcclusionEnabled(ui.getRenderSettingsPanel().isAmbientOcclusionEnabled());
    rayMarcher.setAntiAliasingEnabled(ui.getRenderSettingsPanel().isAntiAliasingEnabled());
    rayMarcher.render();
    rayMarcher.show(static_cast<ray_march::Tex>(static_cast<int>(ui.getRenderSettingsPanel().getSelectedTextureType())));
    ui.onFrame();
    window->swap();
    time += 1 / 60.0f * ui.getRenderSettingsPanel().getTimeScale();
  });
  spdlog::info("Starting main loop");
  (*mainLoop)();
  return 0;
}