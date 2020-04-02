//
// Created by petr on 10/30/19.
//

#ifndef TERRAINGENERATION_GL_UTILS_H
#define TERRAINGENERATION_GL_UTILS_H

#include <geGL/StaticCalls.h>
#include <geGL/geGL.h>

auto checkProgramLinkStatus(GLuint program) -> bool;

auto getGPUMemoryUsage() -> std::pair<uint, uint>;

class ScopedShaderProgramUsage {
public:
  explicit ScopedShaderProgramUsage(ge::gl::Program &program);

  auto operator*() -> ge::gl::Program &;
  auto operator-> () -> ge::gl::Program *;

  ~ScopedShaderProgramUsage();

private:
  ge::gl::Program &program;
};

#endif // TERRAINGENERATION_GL_UTILS_H
