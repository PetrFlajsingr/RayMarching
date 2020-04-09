//
// Created by petr on 10/23/19.
//

#ifndef TERRAINGENERATION_GLSLSHADERLOADER_H
#define TERRAINGENERATION_GLSLSHADERLOADER_H

#include <error_handling/exceptions.h>
#include <geGL/geGL.h>
#include <string>
#include <utility>
#include <various/StringUtils.h>

enum class ShaderType { Vertex, TesselationControl, TesselationEvaluation, Geometry, Fragment, Compute, Include };
auto glEnumToShaderType(GLenum type) -> ShaderType;
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

auto getShaderLocation() -> std::string_view;
auto setShaderLocation(std::string_view newShaderLocation) -> void;

auto setTempShaderLocation(std::string_view newShaderLocation) -> LocationResetter;

auto loadShaderFile(std::string_view name, ShaderType type) -> std::string;

template <typename... IncPaths> inline auto loadShader(GLenum type, const std::string &shaderPath, const IncPaths &... incPaths)
    -> ShaderPtr {
  using namespace std::string_literals;
  const auto shaderType = glEnumToShaderType(type);
  auto mainShaderSrc = loadShaderFile(shaderPath, shaderType);
  std::array includePaths{incPaths...};
  std::array<std::string, includePaths.size()> includeSrcs;
  for (std::size_t i = 0; i < includePaths.size(); ++i) {
    includeSrcs[i] = loadShaderFile(includePaths[i], ShaderType::Include);
  }

  std::size_t incPos = 1;
  auto splitSrc = StringUtils::split(mainShaderSrc, "\n"s);
  auto splitSrcIter = splitSrc.begin();
  auto line = *splitSrcIter++;
  while (splitSrcIter != splitSrc.end() && line[0] == '#') {
    incPos += line.length();
    line = *splitSrcIter++;
  }
  for (const auto &incSrc : includeSrcs) {
    std::string toInsert{"\n"s + incSrc + "\n"};
    mainShaderSrc.insert(incPos, toInsert);
  }
  // std::cout << mainShaderSrc << std::endl;
  return std::make_shared<ge::gl::Shader>(type, mainShaderSrc);
}

#endif // TERRAINGENERATION_GLSLSHADERLOADER_H
