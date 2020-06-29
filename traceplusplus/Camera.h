#pragma once
/*
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Scene.h"
#include "Renderer.h"

#include <climits>

class CCamera
{
public:
  CCamera(
    const glm::fvec3& position,
    const glm::fvec3& forward,
    const glm::fvec3& up)
    : mPosition(position)
    , mForward(forward)
    , mUp(up)
  {
    mPositionTensor = glm::identity<glm::fmat4>();
    mRotationTensor = glm::identity<glm::fmat4>();
    mScaleTensor = glm::identity<glm::fmat4>();
  }

  static inline glm::fvec2 NDC(const glm::fvec2& coords, const glm::fvec2& screenSize) { return coords / screenSize - 0.5f; }

  void Update()
  {
    mPositionTensor = glm::translate(mPositionTensor, mPosition);

    mRotationTensor = glm::rotate(mRotationTensor, glm::radians(mRotation.x), glm::fvec3{ 1, 0, 0 });
    mRotationTensor = glm::rotate(mRotationTensor, glm::radians(mRotation.y), glm::fvec3{ 0, 1, 0 });
    mRotationTensor = glm::rotate(mRotationTensor, glm::radians(mRotation.z), glm::fvec3{ 0, 0, 1 });
  }
  glm::fvec3 ProjectToWorld(const glm::fvec2& rayOrig)
  {
    return mPositionTensor * mRotationTensor * glm::fvec4{ rayOrig, 0.f, 1.f };
  }
  bool Trace(const glm::fvec2& rayOrig, int steps, const CScene& scene, glm::fvec3& hitNormal) const
  {
    glm::fvec3 rayDir = mForward;
    rayDir = mRotationTensor * glm::fvec4{ rayDir, 1.f };

    float currDistance = 1.f;
    float nextDistance = 0.f;

    glm::fvec3 rayDelta = { 0, 0, 0 };

    for (int i = 0; i < steps; i++)
    {
      rayDelta = rayOrig + rayDir * currDistance;

      nextDistance = scene.Distance(rayDelta);

      if (nextDistance < std::numeric_limits<float>::epsilon())
      {
        hitNormal = Gradient(rayOrig, scene);
        return true;
      }

      currDistance += nextDistance;

      if (currDistance >= 1000.f) return false;
    }

    return false;
  }

private:
  glm::fvec3 mUp;
  glm::fvec3 mForward;

  glm::fvec3 mPosition;
  glm::fvec3 mRotation;

  glm::fmat4 mPositionTensor;
  glm::fmat4 mRotationTensor;
  glm::fmat4 mScaleTensor;
};
*/