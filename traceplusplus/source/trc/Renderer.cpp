#include "glad/glad.h"

#include "imgui.h"

#include "trc/Renderer.h"

void TRenderer::Render(const ACS::Components::TCamera* pCamera)
{
  for (const auto& job : lambertPass)
  {
    glUseProgram(job.pShader->program);
    glBindVertexArray(job.pVertexLayout->vao);

    u32 blockIdx = glGetProgramResourceIndex(job.pShader->program, GL_SHADER_STORAGE_BLOCK, "bTransformations");
    glShaderStorageBlockBinding(job.pShader->program, blockIdx, 0);

    s32 uProjection = glGetUniformLocation(job.pShader->program, "uProjection");
    s32 uView = glGetUniformLocation(job.pShader->program, "uView");

    glUniformMatrix4fv(uProjection, 1, 0, &pCamera->projectionMatrix[0][0]);
    glUniformMatrix4fv(uView, 1, 0, &pCamera->viewMatrix[0][0]);

    for (const auto& technic : lambertTechnics)
    {
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

  for (const auto& job : lambertPass)
  {
    if (ImGui::TreeNodeEx(&job, ImGuiTreeNodeFlags_DefaultOpen, "%p", &job))
    {
      ImGui::LabelText("Vao", "%d", job.pVertexLayout->vao);
      ImGui::LabelText("Program", "%d", job.pShader->program);

      ImGui::TreePop();
    }
  }

  ImGui::End();
}