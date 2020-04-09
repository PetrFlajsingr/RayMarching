//
// Created by petr on 2/12/20.
//
#include "UI/UI.h"
#include "common/GlslShaderLoader.h"
#include "ray_marching/CSG/CSGTree.h"
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
#include <various/overload.h>

auto getDisplaySize() -> std::pair<unsigned int, unsigned int> {
  SDL_DisplayMode displayMode;
  if (SDL_GetDesktopDisplayMode(0, &displayMode) != 0) {
    throw exc::Error("SDL_GetDesktopDisplayMode failed");
  }
  const auto w = static_cast<unsigned int>(displayMode.w * 0.8);
  const auto h = static_cast<unsigned int>(displayMode.h * 0.8);
  return {w, h};
}

auto maketree() -> CSGTree {
  CSGTree tree;
  tree.root = std::make_unique<WarpOperationNode>(
      std::make_unique<LimitedSpaceRepetitionOperation>(glm::vec3{500.0, 1000.0, 500.0}, glm::vec3{10.0, 0.0, 10.0}));
  auto &first = dynamic_cast<WarpOperationNode &>(*tree.root);

  auto &firstOp = first.setChild<OperationBlend>(250);

  auto &op = dynamic_cast<OperationCSGNode &>(firstOp.setLeftChild<OperationBlend>(100));

  auto &tower = op.setRightChild<OperationBlend>(500);

  tower.setRightChild<BoxShape>(glm::vec3{0, -12, 0}, glm::vec3{4, 500, 4});
  tower.setLeftChild<SphereShape>(glm::vec3{0, 500, 0}, 10);
  op.setLeftChild<BoxShape>(glm::vec3{0, -15, 0}, glm::vec3{30, 1, 30});

  firstOp.setRightChild<PlaneShape>(glm::vec3{0, -10, 0}, glm::vec4{0, 1.4, 0, 10});

  std::cout << tree.src() << std::endl;
  return tree;
}

auto main() -> int {
  auto tree = maketree();

  spdlog::set_level(spdlog::level::debug);
  auto mainLoop = std::make_shared<sdl2cpp::MainLoop>();
  auto [screenWidth, screenHeight] = getDisplaySize();
  screenWidth *= 0.8;
  screenHeight *= 0.8;
  spdlog::info("Window size: {}x{}", static_cast<int>(screenWidth), static_cast<int>(screenHeight));
  auto window = std::make_shared<sdl2cpp::Window>(screenWidth, screenHeight);
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
  window->setEventCallback(SDL_MOUSEMOTION, [&isCameraControlled, &camera](const SDL_Event &event) {
    if (isCameraControlled) {
      camera.ProcessMouseMovement(event.motion.xrel, event.motion.yrel);
      return true;
    }
    return false;
  });
  window->setEventCallback(SDL_MOUSEBUTTONDOWN, [&isCameraControlled](const SDL_Event &event) {
    isCameraControlled = event.button.button == SDL_BUTTON_RIGHT;
    return isCameraControlled;
  });
  window->setEventCallback(SDL_MOUSEBUTTONUP, [&isCameraControlled](const SDL_Event &event) {
    if (isCameraControlled && event.button.button == SDL_BUTTON_RIGHT) {
      isCameraControlled = false;
      return true;
    }
    return false;
  });

  // glm::vec3 velocity{0};
  window->setEventCallback(SDL_KEYDOWN, [&camera](const SDL_Event &event) {
    constexpr auto movementSpeed = 10.0f;
    const auto pressedKey = event.key.keysym.sym;
    switch (pressedKey) {
    case SDLK_w:
      // velocity += 0.01f * camera.Front;
      camera.ProcessKeyboard(Camera_Movement::FORWARD, movementSpeed);
      break;
    case SDLK_a:
      // velocity += -0.01f * camera.Right;
      camera.ProcessKeyboard(Camera_Movement::RIGHT, movementSpeed);
      break;
    case SDLK_s:
      // velocity += 0.01f * camera.Front;
      camera.ProcessKeyboard(Camera_Movement::BACKWARD, movementSpeed);
      break;
    case SDLK_d:
      // velocity += 0.01f * camera.Right;
      camera.ProcessKeyboard(Camera_Movement::LEFT, movementSpeed);
      break;
    case SDLK_SPACE:
      // velocity += glm::vec3{0, 0.5f, 0};
      camera.Position += camera.Up * movementSpeed;
      break;
    }

    return true;
  });

  window->setWindowEventCallback(SDL_WINDOWEVENT_RESIZED, [&screenWidth, &screenHeight, &rayMarcher](const SDL_Event &event) {
    auto diff = screenWidth - event.window.data1;
    diff += screenHeight - event.window.data2;
    if (diff > 50) {
      screenWidth = event.window.data1;
      screenHeight = event.window.data2;
      rayMarcher.changeRenderSize({screenWidth, screenHeight});
    }
    return true;
  });

  glm::vec3 lastCamPos = camera.Position;
  ui.getRenderSettingsPanel().setOnReloadShaderClicked([&rayMarcher] { rayMarcher.reloadShader(); });

  camera.Position = glm::vec3{0, 400, 0};
  mainLoop->setIdleCallback([&]() {
    ge::gl::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // velocity += glm::vec3{0, -0.009, 0};
    // camera.Position += velocity;
    // if (glm::length(velocity) > 10) {
    //  velocity = glm::normalize(velocity) * 10.0f;
    //}

    float currentDistance = tree.eval(camera.Position);
    if (!ui.getCameraPanel().isClippingEnabled()) {
      unsigned int iterationCount = 0;
      while (currentDistance < 0) {
        const auto surfaceNormal = tree.getNormal(camera.Position);
        const auto delta = surfaceNormal * glm::dot(lastCamPos - camera.Position, surfaceNormal);
        camera.Position += delta;
        camera.Position += surfaceNormal * 0.0001f * float(iterationCount);
        currentDistance = tree.eval(camera.Position);
        iterationCount += 100;
        // if (-velocity.y > 0.1f) {
        // velocity = glm::reflect(velocity, surfaceNormal) * 0.8f;
        //} else {
        // velocity -= surfaceNormal * glm::dot(velocity, surfaceNormal);
        //}
      }
      lastCamPos = camera.Position;
    }
    ui.getCameraPanel().setDistance(currentDistance);

    const auto swapInterval = ui.getFPSPanel().isVsyncEnabled() ? 1 : 0;
    SDL_GL_SetSwapInterval(swapInterval);

    const auto currentFps = ui.getFPSPanel().getFPSCounter().current();

    rayMarcher.setRayStepLimit(ui.getRenderSettingsPanel().getRayStepLimit());
    rayMarcher.setShadowRayStepLimit(ui.getRenderSettingsPanel().getShadowRayStepLimit());
    rayMarcher.setMaxDrawDistance(ui.getRenderSettingsPanel().getMaxDrawDistance());
    rayMarcher.setTime(time);
    rayMarcher.setCameraVec(camera.Position, camera.Front);
    rayMarcher.setAmbientOcclusionEnabled(ui.getRenderSettingsPanel().isAmbientOcclusionEnabled());
    rayMarcher.setAntiaAliasingType(ui.getRenderSettingsPanel().getAAType());
    rayMarcher.setShadowType(ui.getRenderSettingsPanel().getShadowType());
    rayMarcher.setAASize(ui.getRenderSettingsPanel().getAA());
    rayMarcher.setReflectionsEnabled(ui.getRenderSettingsPanel().areReflectionsEnabled());
    rayMarcher.setMaxReflections(ui.getRenderSettingsPanel().getMaxReflections());
    rayMarcher.render();
    rayMarcher.show(ui.getRenderSettingsPanel().getSelectedTextureType());
    ui.onFrame();
    window->swap();
    time += 1 / currentFps * ui.getRenderSettingsPanel().getTimeScale();
  });
  spdlog::info("Starting main loop");
  (*mainLoop)();
  return 0;
}