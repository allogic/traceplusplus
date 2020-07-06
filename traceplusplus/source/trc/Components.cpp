#include "trc/Components.h"

r32m4 TCamera::Projection(r32 aspect) const
{
  switch (projection)
  {
  case TProjection::Perspective:  return glm::perspective(fov, aspect, 0.001f, 1000.f);
  case TProjection::Orthographic: return glm::ortho(-1.f, 1.f, -1.f, 1.f);
  }

  return glm::identity<r32m4>();
}