//
// Created by petr on 2/12/20.
//
#include "UI/UI.h"
#include "common/GlslShaderLoader.h"
#include "ray_marching/CSG/CSGTree.h"
#include "ray_marching/RayMarcher.h"
#include "ray_marching/SceneManager.h"
#include "simulation/PhysicsObject.h"
#include "simulation/PhysicsSimulator.h"
#include <Camera.h>
#include <SDL2CPP/MainLoop.h>
#include <SDL2CPP/Window.h>
#include <SDL_video.h>
#include <argparse.hpp>
#include <error_handling/exceptions.h>
#include <fstream>
#include <geGL/DebugMessage.h>
#include <geGL/StaticCalls.h>
#include <geGL/geGL.h>
#include <memory>
#include <spdlog/spdlog.h>
#include <sstream>
#include <types/Range.h>
#include <utility>
#include <various/overload.h>
using namespace std::string_literals;

class SphereCollisionObject : public SDFCollisionObject {
public:
  SphereCollisionObject(const glm::vec3 &position, float mass) : SDFCollisionObject(position, mass) {}
};

auto getDisplaySize() -> std::pair<unsigned int, unsigned int> {
  SDL_DisplayMode displayMode;
  if (SDL_GetDesktopDisplayMode(0, &displayMode) != 0) {
    throw exc::Error("SDL_GetDesktopDisplayMode failed");
  }
  const auto w = static_cast<unsigned int>(displayMode.w * 0.8);
  const auto h = static_cast<unsigned int>(displayMode.h * 0.8);
  return {w, h};
}

auto loadMaterials(MaterialManager &materialManager, const std::string &assetsPath) -> void {
  auto materialFileStream = std::ifstream{assetsPath + "/materials.json"};
  auto materialJson = nlohmann::json{};
  materialFileStream >> materialJson;
  materialManager.loadFromJson(materialJson);
}

auto main(int argc, char **argv) -> int {
  argparse::ArgumentParser argParser("Ray marching");
  argParser.add_argument("-assets").help("path to assets folder").required();
  argParser.add_argument("-shaders").help("path to shaders folder").required();
  try {
    argParser.parse_args(argc, argv);
  } catch (const std::runtime_error &err) {
    std::cout << err.what() << std::endl;
    std::cout << argParser;
    return 0;
  }
  const std::string shaderPath = argParser.get<std::string>("-shaders");
  const std::string assetsPath = argParser.get<std::string>("-assets");

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

  setShaderLocation(shaderPath);
  ray_march::RayMarcher rayMarcher{{screenWidth, screenHeight}};
  loadMaterials(rayMarcher.getMaterialManager(), assetsPath);
  SceneManager sceneManager;
  for (auto i : MakeRange::range('1', '4')) {
    std::ifstream ifstream{assetsPath + "/scenes/scene"s + i + ".json"};
    nlohmann::json sceneJson;
    ifstream >> sceneJson;
    sceneManager.loadFromJson(sceneJson, rayMarcher.getMaterialManager());
  }
  std::ifstream ifstream{assetsPath + "/scenes/scene_spheres.json"};
  nlohmann::json sceneJson;
  ifstream >> sceneJson;
  sceneManager.loadFromJson(sceneJson, rayMarcher.getMaterialManager());
  auto mainScene = sceneManager.getScene("scene1");
  PhysicsSimulator simulation{};

  float time = 0;

  ui::UI ui{*window, *mainLoop, "450", rayMarcher.getMaterialManager(), mainScene->getCamera(), sceneManager};
  bool isCameraControlled = false;
  window->setEventCallback(SDL_MOUSEMOTION, [&isCameraControlled, &mainScene](const SDL_Event &event) {
    if (isCameraControlled) {
      mainScene->getCamera().ProcessMouseMovement(event.motion.xrel, event.motion.yrel);
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
  auto movementSpeed = 10.0f;
  window->setEventCallback(SDL_KEYDOWN, [&movementSpeed, &mainScene, &simulation](const SDL_Event &event) {
    const auto pressedKey = event.key.keysym.sym;
    // const Uint8 *keyboard_state_array = SDL_GetKeyboardState(NULL);

    switch (pressedKey) {
    case SDLK_w:
      // velocity += 0.01f * camera.Front;
      mainScene->getCamera().ProcessKeyboard(Camera_Movement::FORWARD, movementSpeed);
      break;
    case SDLK_a:
      // velocity += -0.01f * camera.Right;
      mainScene->getCamera().ProcessKeyboard(Camera_Movement::RIGHT, movementSpeed);
      break;
    case SDLK_s:
      // velocity += 0.01f * camera.Front;
      mainScene->getCamera().ProcessKeyboard(Camera_Movement::BACKWARD, movementSpeed);
      break;
    case SDLK_d:
      // velocity += 0.01f * camera.Right;
      mainScene->getCamera().ProcessKeyboard(Camera_Movement::LEFT, movementSpeed);
      break;
    case SDLK_SPACE:
      // velocity += glm::vec3{0, 0.5f, 0};
      mainScene->getCamera().Position += mainScene->getCamera().Up * movementSpeed;
      break;
    case SDLK_TAB:
      auto obj =
          std::make_shared<SphereCollisionObject>(mainScene->getCamera().Position + mainScene->getCamera().Front * 20.0f, 100);
      obj->setVelocity(mainScene->getCamera().Front * 1000.0f);
      simulation.addObject(obj);
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

  glm::vec3 lastCamPos = mainScene->getCamera().Position;
  ui.getRenderSettingsPanel().setOnReloadShaderClicked([&rayMarcher] { rayMarcher.reloadShader(); });

  mainScene->getCamera().Position = glm::vec3{0, 400, 0};
  ui.onFrame();
  mainLoop->setIdleCallback([&]() {
    ge::gl::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    std::chrono::microseconds simTime;
    if (ui.getScenePanel().isChoiceChanged()) {
      mainScene = ui.getScenePanel().getSelectedScene();
    }
    if (ui.getScenePanel().isChoiceChanged()) {
      simulation.setScene(mainScene);
    }
    {
      const auto begin = std::chrono::steady_clock::now();
      simulation.update(time);
      std::vector<glm::vec4> spherePositions;
      for (auto &obj : simulation.getObjects()) {
        spherePositions.emplace_back(glm::vec4{obj->getPosition(), 1});
      }
      rayMarcher.physicsSphereCount = spherePositions.size();
      if (rayMarcher.physicsSphereCount > 0) {
        rayMarcher.buffer.setData(spherePositions);
      }

      movementSpeed = ui.getCameraPanel().getMovementSpeed();
      // velocity += glm::vec3{0, -0.009, 0};
      // camera.Position += velocity;
      // if (glm::length(velocity) > 10) {
      //  velocity = glm::normalize(velocity) * 10.0f;
      //}

      float currentDistance = mainScene->getDistanceToScene(mainScene->getCamera().Position);
      if (!ui.getCameraPanel().isClippingEnabled()) {
        unsigned int iterationCount = 0;
        while (currentDistance < 0) {
          const auto surfaceNormal = mainScene->getNormal(mainScene->getCamera().Position);
          const auto delta = surfaceNormal * glm::dot(lastCamPos - mainScene->getCamera().Position, surfaceNormal);
          mainScene->getCamera().Position += delta;
          mainScene->getCamera().Position += surfaceNormal * 0.0001f * float(iterationCount);
          currentDistance = mainScene->getDistanceToScene(mainScene->getCamera().Position);
          iterationCount += 100;
          // if (-velocity.y > 0.1f) {
          // velocity = glm::reflect(velocity, surfaceNormal) * 0.8f;
          //} else {
          // velocity -= surfaceNormal * glm::dot(velocity, surfaceNormal);
          //}
        }
        lastCamPos = mainScene->getCamera().Position;
      }
      ui.getCameraPanel().setDistance(currentDistance);
      const auto end = std::chrono::steady_clock::now();
      simTime = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
    }

    const auto swapInterval = ui.getFPSPanel().isVsyncEnabled() ? 1 : 0;
    SDL_GL_SetSwapInterval(swapInterval);

    const auto currentFps = ui.getFPSPanel().getFPSCounter().current();

    std::chrono::microseconds renderTime;
    {
      const auto begin = std::chrono::steady_clock::now();
      rayMarcher.setRayStepLimit(ui.getRenderSettingsPanel().getRayStepLimit());
      rayMarcher.setShadowRayStepLimit(ui.getRenderSettingsPanel().getShadowRayStepLimit());
      rayMarcher.setMaxDrawDistance(ui.getRenderSettingsPanel().getMaxDrawDistance());
      rayMarcher.setTime(time);
      rayMarcher.setAmbientOcclusionEnabled(ui.getRenderSettingsPanel().isAmbientOcclusionEnabled());
      rayMarcher.setAntiaAliasingType(ui.getRenderSettingsPanel().getAAType());
      rayMarcher.setShadowType(ui.getRenderSettingsPanel().getShadowType());
      rayMarcher.setAASize(ui.getRenderSettingsPanel().getAA());
      rayMarcher.setMaxReflections(ui.getRenderSettingsPanel().getMaxReflections());
      rayMarcher.setLightPosition(ui.getRenderSettingsPanel().getLightPosition());
      rayMarcher.setUseOptimisedMarching(ui.getRenderSettingsPanel().isUseOptimisedRayMarching());
      rayMarcher.setPixelRadius(ui.getRenderSettingsPanel().getPixelRadius());
      rayMarcher.setRelaxationParameter(ui.getRenderSettingsPanel().getRelaxationParameter());
      rayMarcher.setLogStepCount(ui.getRenderSettingsPanel().isLogStepCount());
      rayMarcher.render(mainScene);
      rayMarcher.show(ui.getRenderSettingsPanel().getSelectedTextureType());
      ui.onFrame();
      window->swap();
      const auto end = std::chrono::steady_clock::now();
      renderTime = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
    }
    time += 1 / currentFps * ui.getRenderSettingsPanel().getTimeScale();
  });
  spdlog::info("Starting main loop");
  (*mainLoop)();
  return 0;
}