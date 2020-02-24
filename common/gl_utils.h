//
// Created by petr on 10/30/19.
//

#ifndef TERRAINGENERATION_GL_UTILS_H
#define TERRAINGENERATION_GL_UTILS_H

#include <geGL/StaticCalls.h>
#include <geGL/geGL.h>

bool checkProgramLinkStatus(GLuint program);

std::pair<uint, uint> getGPUMemoryUsage();

class ScopedShaderProgramUsage {
public:
  explicit ScopedShaderProgramUsage(ge::gl::Program &program);

  ge::gl::Program &operator*();
  ge::gl::Program *operator->();

  ~ScopedShaderProgramUsage();

private:
  ge::gl::Program &program;
};

#endif // TERRAINGENERATION_GL_UTILS_H
