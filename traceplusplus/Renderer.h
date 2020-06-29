#pragma once

#include <glm/glm.hpp>

#include "Scene.h"

glm::fvec3 Gradient(const glm::fvec3& position, const CScene& scene, float gradStep = 0.01f)
{
  const glm::fvec3 dx = { gradStep, 0, 0 };
  const glm::fvec3 dy = { 0, gradStep, 0 };
  const glm::fvec3 dz = { 0, 0, gradStep };

  glm::fvec3 normal = {
    scene.Distance(position + dx) - scene.Distance(position - dx),
    scene.Distance(position + dy) - scene.Distance(position - dy),
    scene.Distance(position + dz) - scene.Distance(position - dz)
  };

  return glm::normalize(normal);
}