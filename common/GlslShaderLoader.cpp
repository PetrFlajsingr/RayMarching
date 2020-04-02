//
// Created by petr on 10/23/19.
//

#include "GlslShaderLoader.h"
#include "error_handling/exceptions.h"
#include <fstream>
#include <iostream>
#define SHADER_FOLDER std::string("/home/petr/CLionProjects/TerrainGeneration/marching_cubes/shaders")

using namespace std::string_literals;
auto loadShaderFile(std::string_view name, ShaderType type) -> std::string {
  if (shaderLocation.empty()) {
    throw exc::ProgrammingError("Shader location for loadShaderFile has not been set");
  }
  std::string extension;
  switch (type) {
  case ShaderType::Vertex:
    extension = ".vert";
    break;
  case ShaderType::TesselationControl:
    extension = ".tesc";
    break;
  case ShaderType::TesselationEvaluation:
    extension = ".tese";
    break;
  case ShaderType::Geometry:
    extension = ".geom";
    break;
  case ShaderType::Fragment:
    extension = ".frag";
    break;
  case ShaderType::Compute:
    extension = ".comp";
    break;
  case ShaderType::Include:
    extension = ".glsl";
    break;
  }
  const auto pathToFile = shaderLocation + "/" + std::string(name) + extension;
  std::ifstream inStream(pathToFile);
  if (!inStream.is_open()) {
    throw std::runtime_error("Shader opening failed: "s + std::string(name));
  }
  return std::string(std::istreambuf_iterator<char>(inStream), std::istreambuf_iterator<char>());
}
auto setShaderLocation(std::string_view newShaderLocation) -> void {
  shaderLocation = std::string(newShaderLocation);
  if (shaderLocation.back() == '/') {
    shaderLocation = shaderLocation.substr(0, shaderLocation.length() - 1);
  }
}
auto getShaderLocation() -> std::string_view { return shaderLocation; }
auto setTempShaderLocation(std::string_view newShaderLocation) -> LocationResetter {
  LocationResetter result(shaderLocation);
  setShaderLocation(newShaderLocation);
  return result;
}
auto glEnumToShaderType(GLenum type) -> ShaderType {
  switch (type) {
  case GL_VERTEX_SHADER:
    return ShaderType::Vertex;
  case GL_FRAGMENT_SHADER:
    return ShaderType::Fragment;
  case GL_GEOMETRY_SHADER:
    return ShaderType::Geometry;
  case GL_COMPUTE_SHADER:
    return ShaderType::Compute;
  case GL_TESS_CONTROL_SHADER:
    return ShaderType::TesselationControl;
  case GL_TESS_EVALUATION_SHADER:
    return ShaderType::TesselationEvaluation;
  }
  throw exc::ProgrammingError("Invalid GLenum for shader type.");
}
LocationResetter::LocationResetter(std::string oldLocation) : oldLocation(std::move(oldLocation)) {}
LocationResetter::LocationResetter(LocationResetter &&other) noexcept {
  other.moved = true;
  oldLocation = std::move(other.oldLocation);
}
LocationResetter::~LocationResetter() {
  if (!moved) {
    setShaderLocation(oldLocation);
  }
}
