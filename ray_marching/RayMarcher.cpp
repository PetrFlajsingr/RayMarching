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
    : csProgram("ray_marching/ray_marcher"_comp), renderProgram("ray_marching/render"_vert, "ray_marching/render"_frag),
      renderTexture(GL_TEXTURE_2D, GL_RGBA32F, 0, textureSize.first, textureSize.second),
      stepCountTexture(GL_TEXTURE_2D, GL_R32F, 0, textureSize.first, textureSize.second),
      depthTexture(GL_TEXTURE_2D, GL_R32F, 0, textureSize.first, textureSize.second),  quadVertices({
                                                                                                        -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                                                                                                        1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 1.0f,  -1.0f, 0.0f, 1.0f, 0.0f,
                                                                                                    }),
      quadVBO(sizeof(float) * quadVertices.size(), quadVertices.data()) {
  quadVAO.addAttrib(&quadVBO, 0, 3, GL_FLOAT, 5 * sizeof(float), 0);
  quadVAO.addAttrib(&quadVBO, 1, 2, GL_FLOAT, 5 * sizeof(float), (3 * sizeof(float)));
  spdlog::debug("[Ray_marcher]Creating with texture size {}x{}", textureSize.first, textureSize.second);
  setTextureInterpolation();
}

void RayMarcher::changeRenderSize(const TextureSize &textureSize) {
  spdlog::debug("[Ray_marcher]Changing texture size to {}x{}", textureSize.first, textureSize.second);
  renderTexture = ge::gl::Texture{GL_TEXTURE_2D, GL_RGBA32F, 0, textureSize.first, textureSize.second};
  stepCountTexture = ge::gl::Texture{GL_TEXTURE_2D, GL_R32F, 0, textureSize.first, textureSize.second};
  depthTexture = ge::gl::Texture{GL_TEXTURE_2D, GL_R32F, 0, textureSize.first, textureSize.second};
  setTextureInterpolation();
}

void RayMarcher::setTextureInterpolation() {
  renderTexture.texParameteri(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  renderTexture.texParameteri(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  stepCountTexture.texParameteri(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  stepCountTexture.texParameteri(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  depthTexture.texParameteri(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  depthTexture.texParameteri(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void RayMarcher::bindTextures() {
  renderTexture.bindImage(renderTextureBinding);
  stepCountTexture.bindImage(stepCountTextureBinding);
  depthTexture.bindImage(depthTextureBinding);
}

void RayMarcher::unBindTextures() {
  renderTexture.unbind(renderTextureBinding);
  stepCountTexture.unbind(stepCountTextureBinding);
  depthTexture.unbind(depthTextureBinding);
}

void RayMarcher::render() {
  ScopedShaderProgramUsage scopedProgram{csProgram};
  bindTextures();
  const auto [dispatchX, dispatchY] = getComputeDispatchSize();
  scopedProgram->dispatch(dispatchX, dispatchY);
  unBindTextures();
}
void RayMarcher::show(Tex tex) {
  ScopedShaderProgramUsage scopedProgram{renderProgram};
  quadVAO.bind();
  switch (tex) {
  case Tex::Render:
    renderTexture.bind(0);
    break;
  case Tex::Depth:
    depthTexture.bind(0);
    break;
  case Tex::StepCount:
    stepCountTexture.bind(0);
    break;
  }

  ge::gl::glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

}

std::pair<unsigned int, unsigned int> RayMarcher::getComputeDispatchSize() {
  return {renderTexture.getWidth(0), renderTexture.getHeight(0)};
}
