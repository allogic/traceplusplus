#include "imgui.h"

#include "trc/ACS.h"
#include "trc/Components.h"

void TACS::Debug()
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

            ImGui::InputFloat3("Position", &pTransform->position[0]);
            ImGui::InputFloat3("Rotation", &pTransform->rotation[0]);
            ImGui::InputFloat3("Scale", &pTransform->scale[0]);
          }
          else if (hash == typeid(TCamera).hash_code())
          {
            TCamera* pCamera = (TCamera*)pComponent;

            ImGui::Checkbox("Projection", (b8*)(&pCamera->projection));
            ImGui::LabelText("Right", "{%3.3f, %3.3f, %3.3f}", pCamera->right.x, pCamera->right.y, pCamera->right.z);
            ImGui::LabelText("Up", "{%3.3f, %3.3f, %3.3f}", pCamera->up.x, pCamera->up.y, pCamera->up.z);
            ImGui::LabelText("Forward", "{%3.3f, %3.3f, %3.3f}", pCamera->forward.x, pCamera->forward.y, pCamera->forward.z);
            ImGui::LabelText("Local Right", "{%3.3f, %3.3f, %3.3f}", pCamera->localRight.x, pCamera->localRight.y, pCamera->localRight.z);
            ImGui::LabelText("Local Up", "{%3.3f, %3.3f, %3.3f}", pCamera->localUp.x, pCamera->localUp.y, pCamera->localUp.z);
            ImGui::LabelText("Local Forward", "{%3.3f, %3.3f, %3.3f}", pCamera->localForward.x, pCamera->localForward.y, pCamera->localForward.z);
            ImGui::SliderFloat("Position Speed", &pCamera->positionSpeed, 0.f, 10.f);
            ImGui::SliderFloat("Rotation Speed", &pCamera->rotationSpeed, 0.f, 10.f);
            ImGui::LabelText("Rotation Drag", "{%3.3f, %3.3f}", pCamera->rotationDrag.x, pCamera->rotationDrag.y);
            ImGui::SliderFloat("Rotation Decay", &pCamera->rotationDecay, 0.f, 10.f);
            ImGui::SliderFloat("Rotation Deadzone", &pCamera->rotationDeadzone, 0.f, 1.f);
            ImGui::SliderFloat2("Rotation Velocity", &pCamera->rotationVelocity[0], -100.f, 100.f);
          }

          ImGui::TreePop();
        }
      }

      ImGui::TreePop();
    }
  }

  ImGui::End();
}