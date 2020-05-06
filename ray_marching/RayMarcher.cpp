//
// Created by petr on 2/23/20.
//

#include "RayMarcher.h"
#include "../common/gl_utils.h"
#include "../common/shader_literals.h"
#include <spdlog/spdlog.h>

using namespace ShaderLiterals;
using namespace ray_march;

RayMarcher::RayMarcher(const TextureSize &textureSize)
    : renderProgram("render"_vert, "render"_frag),
      renderTexture(std::make_shared<ge::gl::Texture>(GL_TEXTURE_2D, GL_RGBA32F, 0, textureSize.first, textureSize.second)),
      stepCountTexture(std::make_shared<ge::gl::Texture>(GL_TEXTURE_2D, GL_R32F, 0, textureSize.first, textureSize.second)),
      depthTexture(std::make_shared<ge::gl::Texture>(GL_TEXTURE_2D, GL_R32F, 0, textureSize.first, textureSize.second)),
      quadVertices({
          -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
          1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 1.0f,  -1.0f, 0.0f, 1.0f, 0.0f,
      }),
      quadVBO(sizeof(float) * quadVertices.size(), quadVertices.data()), textureSize(textureSize) {
  reloadShader();
  quadVAO.addAttrib(&quadVBO, 0, 3, GL_FLOAT, 5 * sizeof(float), 0);
  quadVAO.addAttrib(&quadVBO, 1, 2, GL_FLOAT, 5 * sizeof(float), (3 * sizeof(float)));
  spdlog::debug("[Ray_marcher]Creating with texture size {}x{}", textureSize.first, textureSize.second);
  setTextureInterpolation();
}

auto RayMarcher::changeRenderSize(const TextureSize &textureSize) -> void {
  spdlog::debug("[Ray_marcher]Changing texture size to {}x{}", textureSize.first, textureSize.second);
  renderTexture = std::make_shared<ge::gl::Texture>(GL_TEXTURE_2D, GL_RGBA32F, 0, textureSize.first, textureSize.second);
  stepCountTexture = std::make_shared<ge::gl::Texture>(GL_TEXTURE_2D, GL_R32F, 0, textureSize.first, textureSize.second);
  depthTexture = std::make_shared<ge::gl::Texture>(GL_TEXTURE_2D, GL_R32F, 0, textureSize.first, textureSize.second);
  setTextureInterpolation();
  RayMarcher::textureSize = textureSize;
}

auto RayMarcher::setTextureInterpolation() -> void {
  renderTexture->texParameteri(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  renderTexture->texParameteri(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  stepCountTexture->texParameteri(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  stepCountTexture->texParameteri(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  depthTexture->texParameteri(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  depthTexture->texParameteri(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

auto RayMarcher::bindTextures() -> void {
  renderTexture->bindImage(renderTextureBinding);
  stepCountTexture->bindImage(stepCountTextureBinding);
  depthTexture->bindImage(depthTextureBinding);
}

auto RayMarcher::unBindTextures() -> void {
  renderTexture->unbind(renderTextureBinding);
  stepCountTexture->unbind(stepCountTextureBinding);
  depthTexture->unbind(depthTextureBinding);
}
auto RayMarcher::render(const std::shared_ptr<Scene> &scene) -> void {
  ScopedShaderProgramUsage scopedProgram{*csProgramNotOptimised};
  bindTextures();
  const auto cameraPosition = scene->getCamera().Position;
  const auto cameraFront = scene->getCamera().Front;
  ge::gl::glUniform1i(uniformLocations.stepLimit, rayStepLimit);
  ge::gl::glUniform1i(uniformLocations.shadowStepLimit, shadowRayStepLimit);
  ge::gl::glUniform1f(uniformLocations.time, time);
  ge::gl::glUniform1f(uniformLocations.maxDrawDistance, maxDrawDistance);
  ge::gl::glUniform1i(uniformLocations.maxReflections, maxReflections);
  ge::gl::glUniform1f(uniformLocations.AA_size, static_cast<float>(aaSize));
  ge::gl::glUniform1i(uniformLocations.physicsSphereCount, physicsSphereCount);
  ge::gl::glUniform1i(uniformLocations.enableEdgeAA, aaType == AntiAliasing::EdgeAA);
  ge::gl::glUniform2i(uniformLocations.resolution, textureSize.first, textureSize.second);
  ge::gl::glUniform3f(uniformLocations.cameraPosition, cameraPosition.x, cameraPosition.y, cameraPosition.z);
  ge::gl::glUniform3f(uniformLocations.cameraFront, cameraFront.x, cameraFront.y, cameraFront.z);
  ge::gl::glUniform3f(uniformLocations.lightPos, lightPosition.x, lightPosition.y, lightPosition.z);
  ge::gl::glUniform1i(uniformLocations.logStepCount, logStepCount);

  if (useOptimisedMarching) {
    ge::gl::glUniform1f(uniformLocations.pixelRadius, pixelRadius);
    ge::gl::glUniform1f(uniformLocations.relaxationParameter, relaxationParameter);
  }

  scene->updateAndBind(5, 6, 7);

  buffer.bindBase(GL_SHADER_STORAGE_BUFFER, 4);
  materialManager.updateSSBO();
  materialManager.bindBuffer(materialBinding);
  const auto [dispatchX, dispatchY] = getComputeDispatchSize();
  const std::array<GLuint, 3> subroutineIndices{shadowSubroutineIndices[static_cast<int>(shadowType)],
                                                ambientOcclusionEnabled ? aoSubroutineIndices[1] : aoSubroutineIndices[0],
                                                shadowIntensitySubroutineIndices[static_cast<int>(shadowType)]};
  ge::gl::glUniformSubroutinesuiv(GL_COMPUTE_SHADER, subroutineIndices.size(), subroutineIndices.data());
  ge::gl::glDispatchCompute(dispatchX, dispatchY, 1);
  // scopedProgram->dispatch(dispatchX, dispatchY);
  unBindTextures();
}

auto RayMarcher::show(Tex tex) -> void {
  ScopedShaderProgramUsage scopedProgram{renderProgram};
  ge::gl::glViewport(0, 0, textureSize.first, textureSize.second);
  quadVAO.bind();
  auto asHeatMap = false;
  switch (tex) {
  case Tex::Render:
    renderTexture->bind(0);
    break;
  case Tex::Depth:
    depthTexture->bind(0);
    break;
  case Tex::StepCount:
    stepCountTexture->bind(0);
    asHeatMap = true;
    break;
  }
  scopedProgram->set("asHeatMap", asHeatMap);
  scopedProgram->set("enableFXAA", aaType == AntiAliasing::FXAA);
  ge::gl::glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
auto RayMarcher::getComputeDispatchSize() -> std::pair<unsigned int, unsigned int> {
  constexpr double groupSizeX = 8;
  constexpr double groupSizeY = 8;
  return {std::ceil(renderTexture->getWidth(0) / groupSizeX), std::ceil(renderTexture->getHeight(0) / groupSizeY)};
}
auto RayMarcher::setRayStepLimit(int limit) -> void { rayStepLimit = limit; }
auto RayMarcher::setShadowRayStepLimit(int limit) -> void { shadowRayStepLimit = limit; }
auto RayMarcher::setTime(float time) -> void { RayMarcher::time = time; }
auto RayMarcher::setMaxDrawDistance(float distance) -> void { maxDrawDistance = distance; }

auto RayMarcher::setAmbientOcclusionEnabled(bool isAmbientOcclusionEnabled) -> void {
  ambientOcclusionEnabled = isAmbientOcclusionEnabled;
}
auto RayMarcher::setShadowType(Shadows shadowType) -> void { RayMarcher::shadowType = shadowType; }
auto RayMarcher::setAASize(int aaSize) -> void { RayMarcher::aaSize = aaSize; }

auto RayMarcher::setMaxReflections(int maxReflections) -> void { RayMarcher::maxReflections = maxReflections; }
auto RayMarcher::reloadShader() -> void {
  const auto *const defines = useOptimisedMarching
                                  ? "#define CAST_RAY(ray, distanceFactor) castRayOpti(ray, distanceFactor)\n"
                                    "#define CAST_RAY_EDGEAA(ray, distanceFactor) castEdgeAARayOpti(ray, distanceFactor)"
                                  : "#define CAST_RAY(ray, distanceFactor) castRay(ray, distanceFactor)\n"
                                    "#define CAST_RAY_EDGEAA(ray, distanceFactor) castEdgeAARay(ray, distanceFactor)";

  auto tmpShader = loadShader(GL_COMPUTE_SHADER, "ray_marcher", defines, "inc_fractals", "inc_signed_distance_functions",
                              "inc_CSG_operations", "inc_utils", "inc_rm_uniforms", "inc_rm_types");

  auto tmpProgram = std::make_shared<ge::gl::Program>(tmpShader);
  if (tmpProgram->getLinkStatus()) {
    csProgramNotOptimised = tmpProgram;
    shadowSubroutineIndices[0] = ge::gl::glGetSubroutineIndex(csProgramNotOptimised->getId(), GL_COMPUTE_SHADER, "noShadow");
    shadowSubroutineIndices[1] = ge::gl::glGetSubroutineIndex(csProgramNotOptimised->getId(), GL_COMPUTE_SHADER, "rayShadow");
    shadowSubroutineIndices[2] = ge::gl::glGetSubroutineIndex(csProgramNotOptimised->getId(), GL_COMPUTE_SHADER, "softShadow");
    shadowSubroutineIndices[3] = ge::gl::glGetSubroutineIndex(csProgramNotOptimised->getId(), GL_COMPUTE_SHADER, "hardShadow");
    shadowSubroutineLocation =
        ge::gl::glGetSubroutineUniformLocation(csProgramNotOptimised->getId(), GL_COMPUTE_SHADER, "calculateShadow");
    shadowIntensitySubroutineIndices[0] =
        ge::gl::glGetSubroutineIndex(csProgramNotOptimised->getId(), GL_COMPUTE_SHADER, "noShadowIntensity");
    shadowIntensitySubroutineIndices[1] =
        ge::gl::glGetSubroutineIndex(csProgramNotOptimised->getId(), GL_COMPUTE_SHADER, "rayShadowIntensity");
    shadowIntensitySubroutineIndices[2] =
        ge::gl::glGetSubroutineIndex(csProgramNotOptimised->getId(), GL_COMPUTE_SHADER, "softShadowIntensity");
    shadowIntensitySubroutineIndices[3] =
        ge::gl::glGetSubroutineIndex(csProgramNotOptimised->getId(), GL_COMPUTE_SHADER, "hardShadowIntensity");
    shadowIntensitySubroutineLocation =
        ge::gl::glGetSubroutineUniformLocation(csProgramNotOptimised->getId(), GL_COMPUTE_SHADER, "calculateShadowIntensity");
    aoSubroutineIndices[0] = ge::gl::glGetSubroutineIndex(csProgramNotOptimised->getId(), GL_COMPUTE_SHADER, "noAO");
    aoSubroutineIndices[1] = ge::gl::glGetSubroutineIndex(csProgramNotOptimised->getId(), GL_COMPUTE_SHADER, "calcAO");
    aoSubroutineLocation =
        ge::gl::glGetSubroutineUniformLocation(csProgramNotOptimised->getId(), GL_COMPUTE_SHADER, "calculateAmbientOcclusion");
    uniformLocations.stepLimit = csProgramNotOptimised->getUniformLocation("stepLimit");
    uniformLocations.shadowStepLimit = csProgramNotOptimised->getUniformLocation("shadowStepLimit");
    uniformLocations.time = csProgramNotOptimised->getUniformLocation("time");
    uniformLocations.maxDrawDistance = csProgramNotOptimised->getUniformLocation("maxDrawDistance");
    uniformLocations.maxReflections = csProgramNotOptimised->getUniformLocation("maxReflections");
    uniformLocations.AA_size = csProgramNotOptimised->getUniformLocation("AA_size");
    uniformLocations.physicsSphereCount = csProgramNotOptimised->getUniformLocation("physicsSphereCount");
    uniformLocations.enableEdgeAA = csProgramNotOptimised->getUniformLocation("enableEdgeAA");
    uniformLocations.resolution = csProgramNotOptimised->getUniformLocation("resolution");
    uniformLocations.cameraPosition = csProgramNotOptimised->getUniformLocation("cameraPosition");
    uniformLocations.cameraFront = csProgramNotOptimised->getUniformLocation("cameraFront");
    uniformLocations.lightPos = csProgramNotOptimised->getUniformLocation("lightPos");
    uniformLocations.logStepCount = csProgramNotOptimised->getUniformLocation("logStepCount");
    if (useOptimisedMarching) {
      uniformLocations.pixelRadius = csProgramNotOptimised->getUniformLocation("pixelRadius");
      uniformLocations.relaxationParameter = csProgramNotOptimised->getUniformLocation("relaxationParameter");
    }
  } else {
    spdlog::error("Shader loading failed");
  }
}
auto RayMarcher::setAntiaAliasingType(AntiAliasing aaType) -> void { RayMarcher::aaType = aaType; }
auto RayMarcher::getLightPosition() const -> const glm::vec3 & { return lightPosition; }
auto RayMarcher::setLightPosition(const glm::vec3 &lightPosition) -> void { RayMarcher::lightPosition = lightPosition; }
auto RayMarcher::getMaterialManager() -> MaterialManager & { return materialManager; }
auto RayMarcher::isUseOptimisedMarching() const -> bool { return useOptimisedMarching; }
void RayMarcher::setUseOptimisedMarching(bool useOptimisedMarching) {
  if (useOptimisedMarching != RayMarcher::useOptimisedMarching) {
    RayMarcher::useOptimisedMarching = useOptimisedMarching;
    reloadShader();
  }
}
void RayMarcher::setRelaxationParameter(float relaxationParameter) { RayMarcher::relaxationParameter = relaxationParameter; }
void RayMarcher::setPixelRadius(float pixelRadius) { RayMarcher::pixelRadius = pixelRadius; }
void RayMarcher::setLogStepCount(bool logStepCount) { RayMarcher::logStepCount = logStepCount; }
