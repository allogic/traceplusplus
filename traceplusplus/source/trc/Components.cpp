#include "trc/Components.h"

void TCamera::UpdateProjection(r32 aspect)
{
  switch (projection)
  {
  case TProjection::Perspective:  projectionTensor = glm::perspective(fov, aspect, 0.001f, 1000.f);
  case TProjection::Orthographic: projectionTensor = glm::ortho(-1.f, 1.f, -1.f, 1.f);
  }

  projectionTensor = glm::identity<r32m4>();
}

void TCamera::UpdateView(TTransform* pTransform)
{
  viewTensor = glm::lookAt(pTransform->position, pTransform->position + localForward, localUp);
}