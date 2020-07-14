#include "glad/glad.h"

#include "imgui.h"

#include "trc/Renderer.h"

void TRenderer::Render(const ACS::Components::TCamera* pCamera)
{
  for (const auto& job : transformationPass)
  {
    glUseProgram(job.pShaderLayout->program);
    
    u32 blockIdx = glGetProgramResourceIndex(job.pShaderLayout->program, GL_SHADER_STORAGE_BLOCK, "bTransformations");
    glShaderStorageBlockBinding(job.pShaderLayout->program, blockIdx, 0);
  }

  for (const auto& job : lambertPass)
  {
    glUseProgram(job.pShaderLayout->program);
    glBindVertexArray(job.pVertexLayout->vao);

    u32 blockIdx = glGetProgramResourceIndex(job.pShaderLayout->program, GL_SHADER_STORAGE_BLOCK, "bTransformations");
    glShaderStorageBlockBinding(job.pShaderLayout->program, blockIdx, 0);

    s32 uProjection = glGetUniformLocation(job.pShaderLayout->program, "uProjection");
    s32 uView = glGetUniformLocation(job.pShaderLayout->program, "uView");

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
  transformationPass.clear();
  transformationTechnics.clear();

  lambertPass.clear();
  lambertTechnics.clear();
}

void TRenderer::Debug()
{
  ImGui::Begin("Renderer");

  for (const auto& job : transformationPass)
  {
    if (ImGui::TreeNodeEx(&job, ImGuiTreeNodeFlags_DefaultOpen, "%p", &job))
    {
      ImGui::LabelText("Vao", "%d", job.pVertexLayout->vao);
      ImGui::LabelText("Program", "%d", job.pShaderLayout->program);

      ImGui::TreePop();
    }
  }

  for (const auto& job : lambertPass)
  {
    if (ImGui::TreeNodeEx(&job, ImGuiTreeNodeFlags_DefaultOpen, "%p", &job))
    {
      ImGui::LabelText("Vao", "%d", job.pVertexLayout->vao);
      ImGui::LabelText("Program", "%d", job.pShaderLayout->program);

      ImGui::TreePop();
    }
  }

  ImGui::End();
}