//
// Created by petr on 10/23/19.
//

#include "GlslShaderLoader.h"
#include "error_handling/exceptions.h"
#include <fstream>
#include <iostream>
#define SHADER_FOLDER std::string("/home/petr/CLionProjects/TerrainGeneration/marching_cubes/shaders")

using namespace std::string_literals;
std::string loadShaderFile(std::string_view name, ShaderType type) {
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
  }
  const auto pathToFile = shaderLocation + "/" + std::string(name) + extension;
  std::ifstream inStream(pathToFile);
  if (!inStream.is_open()) {
    throw std::runtime_error("Shader opening failed: "s + std::string(name));
  }
  return std::string(std::istreambuf_iterator<char>(inStream), std::istreambuf_iterator<char>());
}
void setShaderLocation(std::string_view newShaderLocation) {
  shaderLocation = std::string(newShaderLocation);
  if (shaderLocation.back() == '/') {
    shaderLocation = shaderLocation.substr(0, shaderLocation.length() - 1);
  }
}
std::string_view getShaderLocation() { return shaderLocation; }
LocationResetter setTempShaderLocation(std::string_view newShaderLocation) {
  LocationResetter result(shaderLocation);
  setShaderLocation(newShaderLocation);
  return result;
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
