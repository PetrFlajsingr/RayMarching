//
// Created by petr on 10/27/19.
//

#ifndef TERRAINGENERATION_SHADER_LITERALS_H
#define TERRAINGENERATION_SHADER_LITERALS_H

#include "GlslShaderLoader.h"
#include <geGL/geGL.h>

namespace ShaderLiterals {
using namespace std::string_literals;
using ShaderPtr = std::shared_ptr<ge::gl::Shader>;

inline auto operator""_vert(const char *name, std::size_t) -> ShaderPtr {
  return std::make_shared<ge::gl::Shader>(GL_VERTEX_SHADER, loadShaderFile(name, ShaderType::Vertex));
}

inline auto operator""_frag(const char *name, std::size_t) -> ShaderPtr {
  return std::make_shared<ge::gl::Shader>(GL_FRAGMENT_SHADER, loadShaderFile(name, ShaderType::Fragment));
}

inline auto operator""_geom(const char *name, std::size_t) -> ShaderPtr {
  return std::make_shared<ge::gl::Shader>(GL_GEOMETRY_SHADER, loadShaderFile(name, ShaderType::Geometry));
}

inline auto operator""_comp(const char *name, std::size_t) -> ShaderPtr {
  return std::make_shared<ge::gl::Shader>(GL_COMPUTE_SHADER, loadShaderFile(name, ShaderType::Compute));
}

inline auto operator""_tesc(const char *name, std::size_t) -> ShaderPtr {
  return std::make_shared<ge::gl::Shader>(GL_TESS_CONTROL_SHADER, loadShaderFile(name, ShaderType::TesselationControl));
}

inline auto operator""_tese(const char *name, std::size_t) -> ShaderPtr {
  return std::make_shared<ge::gl::Shader>(GL_TESS_EVALUATION_SHADER, loadShaderFile(name, ShaderType::TesselationEvaluation));
}
} // namespace ShaderLiterals

#endif // TERRAINGENERATION_SHADER_LITERALS_H
