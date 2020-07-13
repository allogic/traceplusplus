#include "glad/glad.h"

#include "imgui.h"

#include "trc/Renderer.h"

void TRenderer::Submit(const TRenderJob& job, const TLambertTechnic& technic)
{
  lambertPass.emplace(job);
  lambertTechnics.emplace_back(technic);
}

void TRenderer::Render(const ACS::Components::TCamera* pCamera)
{
  for (const auto& job : lambertPass)
  {
    glUseProgram(job.pShaderLayout->program);
    glBindVertexArray(job.pVertexLayout->vao);

    s32 uProjection = glGetUniformLocation(job.pShaderLayout->program, "uProjection");
    s32 uView = glGetUniformLocation(job.pShaderLayout->program, "uView");
    s32 uModel = glGetUniformLocation(job.pShaderLayout->program, "uModel");

    glUniformMatrix4fv(uProjection, 1, 0, &pCamera->projectionMatrix[0][0]);
    glUniformMatrix4fv(uView, 1, 0, &pCamera->viewMatrix[0][0]);

    for (const auto& technic : lambertTechnics)
    {
      // TODO: mov matrix math to GPU
      // only binds and uploads allowed

      technic.pTransform->modelMatrix = glm::identity<r32m4>();

      technic.pTransform->modelMatrix = glm::translate(technic.pTransform->modelMatrix, technic.pTransform->position);
      technic.pTransform->modelMatrix = glm::rotate(technic.pTransform->modelMatrix, technic.pTransform->rotation.x, technic.pTransform->localRight);
      technic.pTransform->modelMatrix = glm::rotate(technic.pTransform->modelMatrix, technic.pTransform->rotation.y, technic.pTransform->localUp);
      technic.pTransform->modelMatrix = glm::rotate(technic.pTransform->modelMatrix, technic.pTransform->rotation.z, technic.pTransform->localForward);
      technic.pTransform->modelMatrix = glm::scale(technic.pTransform->modelMatrix, technic.pTransform->scale);

      glUniformMatrix4fv(uModel, 1, 0, &technic.pTransform->modelMatrix[0][0]);

      glDrawElements(GL_TRIANGLES, job.pVertexLayout->indexCount, GL_UNSIGNED_INT, 0);
    }
  }
}

void TRenderer::Flush()
{
  lambertPass.clear();
  lambertTechnics.clear();
}

void TRenderer::Debug()
{
  ImGui::Begin("Renderer");

  ImGui::BeginGroup();
  for (const auto& job : lambertPass)
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