//
// Created by petr on 2/23/20.
//

#ifndef RAYMARCHING_RAYMARCHER_H
#define RAYMARCHING_RAYMARCHER_H
#include <geGL/Program.h>
#include <geGL/Texture.h>
#include <geGL/VertexArray.h>
#include <geGL/Buffer.h>
#include <utility>
#include <glm/glm.hpp>

namespace ray_march {
enum class Tex{
  Render, Depth, StepCount
};

using TextureSize = std::pair<int, int>;
class RayMarcher {
public:
  explicit RayMarcher(const TextureSize &textureSize);

  auto changeRenderSize(const TextureSize &textureSize) -> void;

  auto render() -> void;
  auto show(Tex tex) -> void;

  auto setRayStepLimit(int limit) -> void;
  auto setTime(float time) -> void;
  auto setMaxDrawDistance(float distance) -> void;
  auto setCameraVec(const glm::vec3 &cameraPosition, const glm::vec3 &cameraFront) -> void;
  auto setAmbientOcclusionEnabled(bool isAmbientOcclusionEnabled) -> void;
  auto setAntiAliasingEnabled(bool isAntiAliasingEnabled) -> void;

private:
  ge::gl::Program csProgram;
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

  int rayStepLimit = 64;
  float time = 0;
  float maxDrawDistance = 10.0f;

  bool ambientOcclusionEnabled = false;
  bool antiAliasingEnabled = false;

  glm::vec3 cameraPosition;
  glm::vec3 cameraFront;
};
} // namespace ray_march

#endif // RAYMARCHING_RAYMARCHER_H
