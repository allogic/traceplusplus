#include "glad/glad.h"

#include "imgui.h"

#include "trc/Renderer.h"

void Renderer::Submit(const TRenderJob& job, const TLambertTechnic& technic)
{
  sLambertPass.emplace(job);
  sLambertTechnics.emplace_back(technic);
}

void Renderer::Render(const TCamera* pCamera)
{
  for (const auto& job : sLambertPass)
  {
    glUseProgram(job.pShaderLayout->program);
    glBindVertexArray(job.pVertexLayout->vao);
    
    s32 uProjection = glGetUniformLocation(job.pShaderLayout->program, "uProjection");
    s32 uView = glGetUniformLocation(job.pShaderLayout->program, "uView");
    s32 uModel = glGetUniformLocation(job.pShaderLayout->program, "uModel");

    glUniformMatrix4fv(uProjection, 1, 0, &pCamera->projectionTensor[0][0]);
    glUniformMatrix4fv(uView, 1, 0, &pCamera->viewTensor[0][0]);

    r32m4 modelTensor = glm::identity<r32m4>();

    for (const auto& technic : sLambertTechnics)
    {
      modelTensor = glm::identity<r32m4>();

      modelTensor = glm::translate(modelTensor, technic.pTransform->position);
      modelTensor = glm::rotate(modelTensor, technic.pTransform->rotation.x, { 1.f, 0.f, 0.f });
      modelTensor = glm::rotate(modelTensor, technic.pTransform->rotation.y, { 0.f, 1.f, 0.f });
      modelTensor = glm::rotate(modelTensor, technic.pTransform->rotation.z, { 0.f, 0.f, 1.f });
      modelTensor = glm::scale(modelTensor, technic.pTransform->scale);

      glUniformMatrix4fv(uModel, 1, 0, &modelTensor[0][0]);
      
      glDrawElements(GL_TRIANGLES, job.pVertexLayout->indexCount, GL_UNSIGNED_INT, 0);
    }
  }
}

void Renderer::Flush()
{
  sLambertPass.clear();
  sLambertTechnics.clear();
}

void Renderer::Debug()
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