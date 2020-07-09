#include "glad/glad.h"

#include "imgui.h"

#include "trc/Renderer.h"

void TRenderer::Render(const r32 deltaTime)
{
  for (const auto& job : sLambertPass)
  {
    glUseProgram(job.pShaderLayout->program);
    glBindVertexArray(job.pVertexLayout->vao);

    //TACS::
    //
    //r32m4 model = glm::identity<r32m4>();
    //
    //model = glm::translate(model, job.pTransform->position);
    //model = glm::rotate(model, pTransform->rotation.x, { 1.f, 0.f, 0.f });
    //model = glm::rotate(model, pTransform->rotation.y, { 0.f, 1.f, 0.f });
    //model = glm::rotate(model, pTransform->rotation.z, { 0.f, 0.f, 1.f });
    //model = glm::scale(model, pTransform->scale);
    //
    //s32 uDeltaTime = glGetUniformLocation(pShader->pShaderLayout->program, "uDeltaTime");
    //s32 uProjection = glGetUniformLocation(pShader->pShaderLayout->program, "uProjection");
    //s32 uView = glGetUniformLocation(pShader->pShaderLayout->program, "uView");
    //s32 uModel = glGetUniformLocation(pShader->pShaderLayout->program, "uModel");
    //
    //glUniform1f(uDeltaTime, deltaTime);
    //glUniformMatrix4fv(uProjection, 1, 0, &pCamera->projectionTensor[0][0]);
    //glUniformMatrix4fv(uView, 1, 0, &pCamera->viewTensor[0][0]);
    //glUniformMatrix4fv(uModel, 1, 0, &model[0][0]);
    //
    //glDrawElements(GL_TRIANGLES, pMesh->pVertexLayout->indexCount, GL_UNSIGNED_INT, 0);
  }
}

void TRenderer::Clear()
{
  sLambertPass.clear();
}

void TRenderer::Debug()
{
  ImGui::Begin("Renderer");

  ImGui::BeginGroup();
  for (const auto& job : sLambertPass)
  {
    if (ImGui::TreeNodeEx(&job, ImGuiTreeNodeFlags_DefaultOpen, "%p", &job))
    {
      ImGui::LabelText("Vao", "%d", job.pVertexLayout->vao);
      ImGui::LabelText("Program", "%d", job.pShaderLayout->program);

      ImGui::TreePop();
    }
  }
  ImGui::EndGroup();

  ImGui::End();
}