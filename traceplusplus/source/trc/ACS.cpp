#include "glad/glad.h"

#include "imgui.h"

#include "trc/ACS.h"
#include "trc/Scene.h"
#include "trc/Renderer.h"

void ACS::Components::TCamera::UpdateProjection(r32 aspect)
{
  switch (projection)
  {
  case TProjection::Perspective:  projectionMatrix = glm::perspective(fov, aspect, near, far); return;
  case TProjection::Orthographic: projectionMatrix = glm::ortho(-1.f, 1.f, -1.f, 1.f); return;
  }

  projectionMatrix = glm::identity<r32m4>();
}

void ACS::Components::TCamera::UpdateView(TTransform* pTransform)
{
  viewMatrix = glm::lookAt(pTransform->position, pTransform->position + pTransform->localForward, pTransform->localUp);
}

ACS::Components::TShader::TShader(TActor* pActor, ShaderLayout::TShaderProgram* pShader)
  : TComponent("Lambert Shader")
  , pTransform(TScene::Act()->Obtain<TTransform>(pActor))
  , pMesh(TScene::Act()->Obtain<TMesh>(pActor))
  , pShader(pShader) {}

void ACS::Components::TShader::Render() const
{
  TRenderer::Act()->Submit(
    TRenderer::TRenderJob{ pMesh->pVertexLayout, pShader },
    TRenderer::TLambertTechnic{ pTransform }
  );
}