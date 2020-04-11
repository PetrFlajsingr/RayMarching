//
// Created by petr on 4/11/20.
//

#ifndef RAYMARCHING_MATERIALPANEL_H
#define RAYMARCHING_MATERIALPANEL_H
#include "../ray_marching/MaterialManager.h"
#include "Panel.h"

namespace ui {
class MaterialPanel : public Panel {
public:
  explicit MaterialPanel(MaterialManager &materialManager);
  void onFrame() override;

private:
  MaterialManager &materialManager;
  const char *currentMaterial;
  std::size_t currentMaterialindex = 0;
  std::vector<std::string> materials;
  std::vector<std::string> materialTypeChoices;
  const char *currentMaterialType;
  Material *selectedMaterial;

  glm::vec3 color{0};
  float reflectivity = 0;
  float refractiveIndex = 1;
  float refractiveFactor = 0;
  float scatterDensity = 0;
};
} // namespace ui

#endif // RAYMARCHING_MATERIALPANEL_H
