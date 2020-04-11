//
// Created by petr on 2/23/20.
//

#include "RayMarcher.h"
#include "../common/gl_utils.h"
#include "../common/shader_literals.h"
#include <glm/gtc/type_ptr.hpp>
#include <spdlog/spdlog.h>

using namespace ShaderLiterals;
using namespace ray_march;

RayMarcher::RayMarcher(const TextureSize &textureSize)
    : csProgram(std::make_shared<ge::gl::Program>(loadShader(
          GL_COMPUTE_SHADER, "ray_marcher", "inc_fractals", "inc_signed_distance_functions", "inc_CSG_operations", "inc_utils"))),
      renderProgram("render"_vert, "render"_frag),
      renderTexture(std::make_shared<ge::gl::Texture>(GL_TEXTURE_2D, GL_RGBA32F, 0, textureSize.first, textureSize.second)),
      stepCountTexture(std::make_shared<ge::gl::Texture>(GL_TEXTURE_2D, GL_R32F, 0, textureSize.first, textureSize.second)),
      depthTexture(std::make_shared<ge::gl::Texture>(GL_TEXTURE_2D, GL_R32F, 0, textureSize.first, textureSize.second)),
      quadVertices({
          -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
          1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 1.0f,  -1.0f, 0.0f, 1.0f, 0.0f,
      }),
      quadVBO(sizeof(float) * quadVertices.size(), quadVertices.data()), textureSize(textureSize) {
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

auto RayMarcher::render() -> void {
  ScopedShaderProgramUsage scopedProgram{*csProgram};
  bindTextures();
  scopedProgram->set("stepLimit", rayStepLimit);
  scopedProgram->set("shadowStepLimit", shadowRayStepLimit);
  scopedProgram->set("time", time);
  scopedProgram->set("maxDrawDistance", maxDrawDistance);
  scopedProgram->set("enableAmbientOcclusion", ambientOcclusionEnabled);
  scopedProgram->set("enableAntiAliasing", aaType == AntiAliasing::SSAA);
  // scopedProgram->set("enableEdgeAntiAliasing", aaType == AntiAliasing::EdgeAA);
  scopedProgram->set("maxReflections", maxReflections);
  scopedProgram->set("shadowType", static_cast<int>(shadowType));
  scopedProgram->set("AA_size", static_cast<float>(aaSize));
  scopedProgram->set2i("resolution", textureSize.first, textureSize.second);
  scopedProgram->set3f("cameraPosition", cameraPosition.x, cameraPosition.y, cameraPosition.z);
  scopedProgram->set3f("cameraFront", cameraFront.x, cameraFront.y, cameraFront.z);
  scopedProgram->set3f("lightPos", lightPosition.x, lightPosition.y, lightPosition.z);
  materialManager.updateSSBO();
  materialManager.bindBuffer(materialBinding);
  const auto [dispatchX, dispatchY] = getComputeDispatchSize();
  scopedProgram->dispatch(dispatchX, dispatchY);
  unBindTextures();
}
auto RayMarcher::show(Tex tex) -> void {
  ScopedShaderProgramUsage scopedProgram{renderProgram};
  ge::gl::glViewport(0, 0, textureSize.first, textureSize.second);
  quadVAO.bind();
  switch (tex) {
  case Tex::Render:
    renderTexture->bind(0);
    break;
  case Tex::Depth:
    depthTexture->bind(0);
    break;
  case Tex::StepCount:
    stepCountTexture->bind(0);
    break;
  }
  scopedProgram->set("enableFXAA", aaType == AntiAliasing::FXAA);

  ge::gl::glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

auto RayMarcher::getComputeDispatchSize() -> std::pair<unsigned int, unsigned int> {
  constexpr double groupSize = 32;
  return {renderTexture->getWidth(0) / groupSize + 1, renderTexture->getHeight(0) / groupSize + 1};
}
auto RayMarcher::setRayStepLimit(int limit) -> void { rayStepLimit = limit; }
auto RayMarcher::setShadowRayStepLimit(int limit) -> void { shadowRayStepLimit = limit; }
auto RayMarcher::setTime(float time) -> void { RayMarcher::time = time; }
auto RayMarcher::setMaxDrawDistance(float distance) -> void { maxDrawDistance = distance; }
auto RayMarcher::setCameraVec(const glm::vec3 &cameraPosition, const glm::vec3 &cameraFront) -> void {
  RayMarcher::cameraPosition = cameraPosition;
  RayMarcher::cameraFront = cameraFront;
}
auto RayMarcher::setAmbientOcclusionEnabled(bool isAmbientOcclusionEnabled) -> void {
  ambientOcclusionEnabled = isAmbientOcclusionEnabled;
}

auto RayMarcher::setShadowType(Shadows shadowType) -> void { RayMarcher::shadowType = shadowType; }
auto RayMarcher::setAASize(int aaSize) -> void { RayMarcher::aaSize = aaSize; }
auto RayMarcher::setMaxReflections(int maxReflections) -> void { RayMarcher::maxReflections = maxReflections; }

auto RayMarcher::reloadShader() -> void {
  auto tmpShader = loadShader(GL_COMPUTE_SHADER, "ray_marcher", "inc_fractals", "inc_signed_distance_functions",
                              "inc_CSG_operations", "inc_utils");
  auto tmpProgram = std::make_shared<ge::gl::Program>(tmpShader);
  if (tmpProgram->getLinkStatus()) {
    csProgram = tmpProgram;
  } else {
    spdlog::error("Shader loading failed");
  }
}
auto RayMarcher::setAntiaAliasingType(AntiAliasing aaType) -> void { RayMarcher::aaType = aaType; }
auto RayMarcher::getLightPosition() const -> const glm::vec3 & { return lightPosition; }
auto RayMarcher::setLightPosition(const glm::vec3 &lightPosition) -> void { RayMarcher::lightPosition = lightPosition; }
auto RayMarcher::getMaterialManager() -> MaterialManager & { return materialManager; }
