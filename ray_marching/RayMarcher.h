//
// Created by petr on 2/23/20.
//

#ifndef RAYMARCHING_RAYMARCHER_H
#define RAYMARCHING_RAYMARCHER_H
#include "MaterialManager.h"
#include "Scene.h"
#include <geGL/Buffer.h>
#include <geGL/Program.h>
#include <geGL/Texture.h>
#include <geGL/VertexArray.h>
#include <glm/glm.hpp>
#include <utility>

namespace ray_march {
enum class Tex { Render = 0, Depth, StepCount };
enum class Shadows : int { Disabled = 0, RayTraced, Soft, Hard };
enum class AntiAliasing : int { Disabled = 0, FXAA, SSAA, EdgeAA };

using TextureSize = std::pair<int, int>;
class RayMarcher {
public:
  explicit RayMarcher(const TextureSize &textureSize);

  auto changeRenderSize(const TextureSize &textureSize) -> void;
  auto render(const std::shared_ptr<Scene> &scene) -> void;
  auto show(Tex tex) -> void;

  auto setRayStepLimit(int limit) -> void;
  auto setShadowRayStepLimit(int limit) -> void;
  auto setTime(float time) -> void;
  auto setMaxDrawDistance(float distance) -> void;
  auto setAmbientOcclusionEnabled(bool isAmbientOcclusionEnabled) -> void;
  auto setShadowType(Shadows shadowType) -> void;
  auto setAASize(int aaSize) -> void;
  auto setMaxReflections(int maxReflections) -> void;
  auto setAntiaAliasingType(AntiAliasing aaType) -> void;
  [[nodiscard]] auto getLightPosition() const -> const glm::vec3 &;
  void setLightPosition(const glm::vec3 &lightPosition);

  [[nodiscard]] auto isUseOptimisedMarching() const -> bool;
  void setUseOptimisedMarching(bool useOptimisedMarching);

  [[nodiscard]] auto getMaterialManager() -> MaterialManager &;

  auto reloadShader() -> void;

  int physicsSphereCount = 0;
  ge::gl::Buffer buffer{1000 * sizeof(int)};

private:
  std::shared_ptr<ge::gl::Program> csProgram;
  ge::gl::Program renderProgram;

  std::shared_ptr<ge::gl::Texture> renderTexture;
  std::shared_ptr<ge::gl::Texture> stepCountTexture;
  std::shared_ptr<ge::gl::Texture> depthTexture;

  static constexpr int renderTextureBinding = 0;
  static constexpr int stepCountTextureBinding = 1;
  static constexpr int depthTextureBinding = 2;

  auto setTextureInterpolation() -> void;
  auto bindTextures() -> void;
  auto unBindTextures() -> void;

  auto getComputeDispatchSize() -> std::pair<unsigned int, unsigned int>;

  std::vector<float> quadVertices;
  ge::gl::VertexArray quadVAO;
  ge::gl::Buffer quadVBO;

  int rayStepLimit = 512;
  int shadowRayStepLimit = 64;
  int maxReflections = 1;
  float time = 0;
  float maxDrawDistance = 10.0f;
  glm::vec3 lightPosition;

  bool useOptimisedMarching = false;

  static inline constexpr unsigned int materialBinding = 3;

  bool ambientOcclusionEnabled = false;

  TextureSize textureSize;

  MaterialManager materialManager;

  Shadows shadowType = Shadows::Disabled;
  AntiAliasing aaType = AntiAliasing::Disabled;
  int aaSize;

  std::array<GLuint, 4> shadowSubroutineIndices;
  std::array<GLuint, 2> aoSubroutineIndices;
  std::array<GLuint, 4> shadowIntensitySubroutineIndices;
  GLuint shadowSubroutineLocation;
  GLuint aoSubroutineLocation;
  GLuint shadowIntensitySubroutineLocation;
};
} // namespace ray_march

#endif // RAYMARCHING_RAYMARCHER_H
