//
// Created by petr on 2/23/20.
//

#ifndef RAYMARCHING_RAYMARCHER_H
#define RAYMARCHING_RAYMARCHER_H
#include <geGL/Program.h>
#include <geGL/Texture.h>
#include <utility>

namespace ray_march {
using TextureSize = std::pair<int, int>;
class RayMarcher {
public:
  explicit RayMarcher(const TextureSize &textureSize);

  void changeRenderSize(const TextureSize &textureSize);

  void render();
  void show();

private:
  ge::gl::Program csProgram;
  ge::gl::Program renderProgram;

  ge::gl::Texture renderTexture;
  ge::gl::Texture stepCountTexture;
  ge::gl::Texture depthTexture;

  static constexpr int renderTextureBinding = 0;
  static constexpr int stepCountTextureBinding = 1;
  static constexpr int depthTextureBinding = 2;

  void setTextureInterpolation();
  void bindTextures();
};
} // namespace ray_march

#endif // RAYMARCHING_RAYMARCHER_H
