#include "imgui.h"

#include "trc/ACS.h"
#include "trc/Components.h"

void ACS::Debug()
{
  ImGui::Begin("ACS");

  for (const auto& [key, components] : sActors)
  {
    auto pActor = (TActor*)(key.id >> 64).to_ullong();

    if (ImGui::TreeNodeEx(pActor, 0, "%s", pActor->name))
    {
      for (const auto& [hash, pComponent] : components)
      {
        if (ImGui::TreeNodeEx(pComponent, 0, "%s", pComponent->name))
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
          }
          else if (hash == typeid(TMesh).hash_code())
          {
            TMesh* pMesh = (TMesh*)pComponent;

            ImGui::LabelText("Vertex Count: ", "%d", pMesh->pVertexLayout->vertexCount);
            ImGui::LabelText("Triangle Count: ", "%d", pMesh->pVertexLayout->indexCount / 3);
          }
          else
          {
            pComponent->Debug();
          }

          ImGui::TreePop();
        }
      }

      ImGui::TreePop();
    }
  }

  ImGui::End();
}