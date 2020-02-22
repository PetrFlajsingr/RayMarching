//
// Created by petr on 10/30/19.
//

#ifndef TERRAINGENERATION_GL_UTILS_H
#define TERRAINGENERATION_GL_UTILS_H

#include <geGL/StaticCalls.h>
#include <geGL/geGL.h>

bool checkProgramLinkStatus(GLuint program);

std::pair<uint, uint> getGPUMemoryUsage();

#endif // TERRAINGENERATION_GL_UTILS_H
