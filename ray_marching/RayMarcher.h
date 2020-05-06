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

  void setRelaxationParameter(float relaxationParameter);
  void setPixelRadius(float pixelRadius);
  void setLogStepCount(bool logStepCount);
  void setUse2Stage(bool use2Stage);

  [[nodiscard]] auto isUseOptimisedMarching() const -> bool;
  void setUseOptimisedMarching(bool useOptimisedMarching);

  [[nodiscard]] auto getMaterialManager() -> MaterialManager &;

  auto reloadShader() -> void;

  int physicsSphereCount = 0;
  ge::gl::Buffer buffer{1000 * sizeof(int)};

private:
  auto renderSingleStage(const std::shared_ptr<Scene> &scene) -> void;
  auto render2Stage(const std::shared_ptr<Scene> &scene) -> void;
  std::shared_ptr<ge::gl::Program> singleStageProgram;
  std::shared_ptr<ge::gl::Program> stage1Program;
  std::shared_ptr<ge::gl::Program> stage2Program;
  ge::gl::Program renderProgram;

  std::shared_ptr<ge::gl::Texture> renderTexture;
  std::shared_ptr<ge::gl::Texture> stepCountTexture;
  std::shared_ptr<ge::gl::Texture> depthTexture;
  std::shared_ptr<ge::gl::Texture> distanceTexture;

  static constexpr int renderTextureBinding = 0;
  static constexpr int stepCountTextureBinding = 1;
  static constexpr int depthTextureBinding = 2;

  static constexpr int pixelStep = 9;

  auto setTextureInterpolation() -> void;
  auto bindTextures(bool useDistanceTexture) -> void;
  auto unBindTextures(bool useDistanceTexture) -> void;

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
  float relaxationParameter = 1.4f;
  float pixelRadius = 0.00001f;

  bool useOptimisedMarching = false;
  bool logStepCount = true;

  bool use2Stage = false;

  static inline constexpr unsigned int materialBinding = 3;

  bool ambientOcclusionEnabled = false;

  TextureSize textureSize;

  MaterialManager materialManager;

  Shadows shadowType = Shadows::Disabled;
  AntiAliasing aaType = AntiAliasing::Disabled;
  int aaSize;

  struct {
    std::array<GLuint, 4> shadowSubroutineIndices;
    std::array<GLuint, 2> aoSubroutineIndices;
    std::array<GLuint, 4> shadowIntensitySubroutineIndices;
  } singleStageSubroutines;
  struct {
    std::array<GLuint, 4> shadowSubroutineIndices;
    std::array<GLuint, 2> aoSubroutineIndices;
    std::array<GLuint, 4> shadowIntensitySubroutineIndices;
  } stage2Subroutines;

  struct {
    GLuint stepLimit;
    GLuint shadowStepLimit;
    GLuint time;
    GLuint maxDrawDistance;
    GLuint maxReflections;
    GLuint AA_size;
    GLuint physicsSphereCount;
    GLuint enableEdgeAA;
    GLuint resolution;
    GLuint cameraPosition;
    GLuint cameraFront;
    GLuint lightPos;
    GLuint logStepCount;
    GLuint pixelRadius;
    GLuint relaxationParameter;
  } singleStageUniformLocations;

  struct {
    GLuint pixelRadius;
    GLuint maxDrawDistance;
    GLuint relaxationParameter;
    GLuint resolution;
    GLuint stepLimit;
    GLuint cameraPosition;
    GLuint cameraFront;
  } stage1UniformLocations;

  struct {
    GLuint stepLimit;
    GLuint shadowStepLimit;
    GLuint time;
    GLuint maxDrawDistance;
    GLuint maxReflections;
    GLuint AA_size;
    GLuint physicsSphereCount;
    GLuint enableEdgeAA;
    GLuint resolution;
    GLuint cameraPosition;
    GLuint cameraFront;
    GLuint lightPos;
    GLuint logStepCount;
    GLuint pixelRadius;
    GLuint relaxationParameter;
  } stage2UniformLocations;
};
} // namespace ray_march

#endif // RAYMARCHING_RAYMARCHER_H
