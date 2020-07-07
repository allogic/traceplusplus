#include "imgui.h"

#include "trc/ACS.h"
#include "trc/Components.h"

void TACS::DebugFoo()
{
  ImGui::Begin("ACS");

  for (auto pActor : sActors)
  {
    if (ImGui::TreeNodeEx(pActor, ImGuiTreeNodeFlags_DefaultOpen, "%p", pActor))
    {
      for (auto [hash, pComponent] : *pActor->pComponents)
      {
        if (ImGui::TreeNodeEx(pComponent, ImGuiTreeNodeFlags_DefaultOpen, "%p", pComponent))
        {
          if (hash == typeid(TTransform).hash_code())
          {
            TTransform* pTransform = (TTransform*)pComponent;

            ImGui::LabelText("Position", "{%3.3f, %3.3f, %3.3f}", pTransform->position.x, pTransform->position.y, pTransform->position.z);
            ImGui::LabelText("Rotation", "{%3.3f, %3.3f, %3.3f}", pTransform->rotation.x, pTransform->rotation.y, pTransform->rotation.z);
            ImGui::LabelText("Scale", "{%3.3f, %3.3f, %3.3f}", pTransform->scale.x, pTransform->scale.y, pTransform->scale.z);
          }

          ImGui::TreePop();
        }
      }

      ImGui::TreePop();
    }
  }

  ImGui::End();
}