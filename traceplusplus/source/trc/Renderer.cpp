#include "imgui.h"

#include "trc/Renderer.h"

void TRenderer::Render(const r32 deltaTime)
{
  //sLambertPass
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
      ImGui::LabelText("Program", "%d", job.program);
      ImGui::LabelText("Vao", "%d", job.vao);

      ImGui::TreePop();
    }
  }
  ImGui::EndGroup();

  ImGui::End();
}