//
// Created by petr on 10/23/19.
//

#ifndef TERRAINGENERATION_GLSLSHADERLOADER_H
#define TERRAINGENERATION_GLSLSHADERLOADER_H

#include <error_handling/exceptions.h>
#include <geGL/geGL.h>
#include <string>
#include <utility>

enum class ShaderType { Vertex, TesselationControl, TesselationEvaluation, Geometry, Fragment, Compute, Include };
ShaderType glEnumToShaderType(GLenum type);
using ShaderPtr = std::shared_ptr<ge::gl::Shader>;

namespace {
std::string shaderLocation;
}
class LocationResetter {
public:
  explicit LocationResetter(std::string oldLocation);
  LocationResetter(const LocationResetter &) = delete;
  LocationResetter(LocationResetter &&other) noexcept;
  ~LocationResetter();

private:
  bool moved = false;
  std::string oldLocation;
};

std::string_view getShaderLocation();

void setShaderLocation(std::string_view newShaderLocation);

LocationResetter setTempShaderLocation(std::string_view newShaderLocation);

std::string loadShaderFile(std::string_view name, ShaderType type);

template <typename... IncPaths>
inline ShaderPtr loadShader(GLenum type, const std::string &shaderPath, const IncPaths &... incPaths) {
  using namespace std::string_literals;
  const auto shaderType = glEnumToShaderType(type);
  auto mainShaderSrc = loadShaderFile(shaderPath, shaderType);
  std::array includePaths{incPaths...};
  std::array<std::string, includePaths.size()> includeSrcs;
  for (std::size_t i = 0; i < includePaths.size(); ++i) {
    includeSrcs[i] = loadShaderFile(includePaths[i], ShaderType::Include);
  }
  for (const auto &incSrc : includeSrcs) {
    std::string toInsert{"\n"s + incSrc + "\n"};
    mainShaderSrc.insert(12, toInsert);
  }
  return std::make_shared<ge::gl::Shader>(type, mainShaderSrc);
}

#endif // TERRAINGENERATION_GLSLSHADERLOADER_H
