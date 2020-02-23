//
// Created by petr on 2/23/20.
//

#include "RayMarcher.h"
#include "../common/shader_literals.h"

using namespace ShaderLiterals;
using namespace ray_march;

RayMarcher::RayMarcher(const TextureSize &textureSize)
    : csProgram("ray_marching/ray_marcher"_comp), renderProgram("ray_marching/render"_vert, "ray_marching/render"_frag),
      renderTexture(GL_TEXTURE_2D, GL_RGBA32F, 0, textureSize.first, textureSize.second),
      stepCountTexture(GL_TEXTURE_2D, GL_R32UI, 0, textureSize.first, textureSize.second),
      depthTexture(GL_TEXTURE_2D, GL_R32F, 0, textureSize.first, textureSize.second) {}

void RayMarcher::changeRenderSize(const TextureSize &textureSize) {
  renderTexture = ge::gl::Texture{GL_TEXTURE_2D, GL_RGBA32F, 0, textureSize.first, textureSize.second};
  stepCountTexture = ge::gl::Texture{GL_TEXTURE_2D, GL_R32UI, 0, textureSize.first, textureSize.second};
  depthTexture = ge::gl::Texture{GL_TEXTURE_2D, GL_R32F, 0, textureSize.first, textureSize.second};
}
