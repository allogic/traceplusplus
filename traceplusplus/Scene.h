#pragma once

#include <glm/glm.hpp>

#include <vector>
#include <utility>
#include <limits>

struct ISdf
{
  virtual float Distance(const glm::fvec3& position) const = 0;
};

class CSphere
  : public ISdf
{
public:
  CSphere(
    const glm::fvec3& position,
    float radius)
    : mPosition(position)
    , mRadius(radius) {}

  virtual float Distance(const glm::fvec3& position) const override
  {
    const float d = glm::length(mPosition - position) - mRadius;
    return d;
  }

private:
  glm::fvec3 mPosition;
  float mRadius;
};

class CCube
  : public ISdf
{
public:
  CCube(
    const glm::fvec3& position,
    const glm::fvec3& size)
    : mPosition(position)
    , mSize(size) {}

  virtual float Distance(const glm::fvec3& position) const override
  {
    const glm::fvec3 distance = glm::abs(mPosition) - mSize;
    float d = glm::min(glm::max(distance.x, glm::max(distance.y, distance.z)), 0.0f) + glm::length(glm::max(distance.x, glm::max(distance.y, distance.z)));
    return d;
  }

private:
  glm::fvec3 mPosition;
  glm::fvec3 mSize;
};

class CScene
{
public:
  CScene() = default;

  void AddObject(const ISdf* pObject) { mObjects.emplace_back(pObject); }

  float Distance(const glm::fvec3 position) const
  {
    float distance = std::numeric_limits<float>::max();

    for (const auto& object : mObjects)
    {
      const float nextDistance = object->Distance(position);

      if (nextDistance < distance)
        distance = nextDistance;
    }

    return distance;
  }

private:
  std::vector<const ISdf*> mObjects;
};