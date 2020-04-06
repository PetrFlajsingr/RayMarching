//
// Created by petr on 4/6/20.
//

#ifndef RAYMARCHING_SCENE_H
#define RAYMARCHING_SCENE_H

#include "CSG/CSGTree.h"

using SceneObject = CSGTree;

class Scene {
public:
  using ObjectId = std::string;

  auto addObject(const ObjectId &id, std::unique_ptr<SceneObject> &&object) { objects[id] = std::move(object); }

  [[nodiscard]] auto getObject(const ObjectId &id) -> SceneObject & {
    if (!objects.contains(id)) {
      // TODO:
      throw "TODO: not found";
    }
    return *objects[id];
  }

private:
  std::unordered_map<ObjectId, std::unique_ptr<SceneObject>> objects;
};

#endif // RAYMARCHING_SCENE_H
