#include "trc/Components.h"

void TCamera::UpdateProjection(r32 aspect)
{
  switch (projection)
  {
  case TProjection::Perspective:  projectionTensor = glm::perspective(fov, aspect, near, far); return;
  case TProjection::Orthographic: projectionTensor = glm::ortho(-1.f, 1.f, -1.f, 1.f); return;
  }

  projectionTensor = glm::identity<r32m4>();
}

void TCamera::UpdateView(TTransform* pTransform)
{
  viewTensor = glm::lookAt(pTransform->position, pTransform->position + localForward, localUp);
}