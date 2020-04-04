//
// Created by petr on 2/23/20.
//

#ifndef RAYMARCHING_RAYMARCHER_H
#define RAYMARCHING_RAYMARCHER_H
#include <geGL/Buffer.h>
#include <geGL/Program.h>
#include <geGL/Texture.h>
#include <geGL/VertexArray.h>
#include <glm/glm.hpp>
#include <utility>

namespace ray_march {
enum class Tex { Render = 0, Depth, StepCount };

enum class Shadows : int { None = 0, RayTraced, Soft, Hard };

using TextureSize = std::pair<int, int>;
class RayMarcher {
public:
  explicit RayMarcher(const TextureSize &textureSize);

  auto changeRenderSize(const TextureSize &textureSize) -> void;

  auto render() -> void;
  auto show(Tex tex) -> void;

  auto setRayStepLimit(int limit) -> void;
  auto setShadowRayStepLimit(int limit) -> void;
  auto setTime(float time) -> void;
  auto setMaxDrawDistance(float distance) -> void;
  auto setCameraVec(const glm::vec3 &cameraPosition, const glm::vec3 &cameraFront) -> void;
  auto setAmbientOcclusionEnabled(bool isAmbientOcclusionEnabled) -> void;
  auto setAntiAliasingEnabled(bool isAntiAliasingEnabled) -> void;
  auto setReflectionsEnabled(bool areRefflectionsEnabled) -> void;
  auto setShadowType(Shadows shadowType) -> void;
  auto setAASize(int aaSize) -> void;
  auto setMaxReflections(int maxReflections) -> void;

  auto reloadShader() -> void;

private:
  std::shared_ptr<ge::gl::Program> csProgram;
  ge::gl::Program renderProgram;

  ge::gl::Texture renderTexture;
  ge::gl::Texture stepCountTexture;
  ge::gl::Texture depthTexture;

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

  bool ambientOcclusionEnabled = false;
  bool antiAliasingEnabled = false;
  bool reflectionsEnabled = false;

  glm::vec3 cameraPosition;
  glm::vec3 cameraFront;

  TextureSize textureSize;

  Shadows shadowType;
  int aaSize;
};
} // namespace ray_march

#endif // RAYMARCHING_RAYMARCHER_H
