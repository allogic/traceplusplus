#pragma once

#include <glm/glm.hpp>

#include <vector>

struct TRay
{
  glm::fvec3 origin;
  glm::fvec3 direction;
};

struct TTransform
{
  glm::fvec3 position;
  glm::fvec3 rotation;
  glm::fvec3 scale;
};

struct TCamera
{
  TTransform transform;
};

namespace sdf
{
  struct TObject
  {
    TTransform transform;

    virtual float Distance(const glm::fvec3& position) = 0;
  };
}

struct TScene
{
  TTransform transform;

  std::vector<sdf::TObject*> objects;
};

//static ProjectRay(glm::fvec3);