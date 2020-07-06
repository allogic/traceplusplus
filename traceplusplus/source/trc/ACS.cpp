#include "imgui.h"

#include "trc/ACS.h"

void TACS::Debug()
{
  ImGui::Begin("ACS");

  for (auto pActor : sActors)
  {
    if (ImGui::TreeNode(pActor, "%p", pActor))
    {
      for (auto [hash, pComponent] : *pActor->pComponents)
      {
        if (ImGui::TreeNode(pComponent, "%d %p", hash, pComponent))
        {
          ImGui::TreePop();
        }
      }

      ImGui::TreePop();
    }
  }

  ImGui::End();
}