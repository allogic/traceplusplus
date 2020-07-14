#include "imgui.h"

#include "trc/Scene.h"

void TScene::Update(const r32 deltaTime)
{
  for (const auto& [key, components] : actors)
    for (const auto& [hash, pComponent] : components)
      pComponent->Update(deltaTime);
}

void TScene::Render()
{
  for (const auto& [key, components] : actors)
    for (const auto& [hash, pComponent] : components)
      pComponent->Render();
}

void TScene::Event(ACS::TEvent* pEvent)
{
  for (const auto& [key, components] : actors)
    for (const auto& [hash, pComponent] : components)
      pComponent->Event(*pEvent);
}

void TScene::Debug()
{
  ImGui::Begin("Scene");

  for (const auto& [key, components] : actors)
  {
    auto pActor = (ACS::TActor*)(key.id >> 64).to_ullong();

    if (ImGui::TreeNodeEx(pActor, 0, "%s", pActor->name))
    {
      for (const auto& [hash, pComponent] : components)
      {
        if (ImGui::TreeNodeEx(pComponent, 0, "%s", pComponent->name))
        {
          if (hash == Register<ACS::Components::TTransform>())
          {
            auto pTransform = (ACS::Components::TTransform*)pComponent;

            ImGui::InputFloat3("Position", &pTransform->position[0]);
            ImGui::InputFloat3("Rotation", &pTransform->rotation[0]);
            ImGui::InputFloat3("Scale", &pTransform->scale[0]);
            ImGui::InputFloat3("Right", &pTransform->right[0]);
            ImGui::InputFloat3("Up", &pTransform->up[0]);
            ImGui::InputFloat3("Forward", &pTransform->forward[0]);
            ImGui::InputFloat3("Local Right", &pTransform->localRight[0]);
            ImGui::InputFloat3("Local Up", &pTransform->localUp[0]);
            ImGui::InputFloat3("Local Forward", &pTransform->localForward[0]);
          }
          else if (hash == Register<ACS::Components::TCamera>())
          {
            auto pCamera = (ACS::Components::TCamera*)pComponent;

            ImGui::Checkbox("Projection", (b8*)(&pCamera->projection));

          }
          else if (hash == Register<ACS::Components::TMesh>())
          {
            auto pMesh = (ACS::Components::TMesh*)pComponent;

            ImGui::LabelText("Vertex Count", "%d", pMesh->pVertexLayout->vertexCount);
            ImGui::LabelText("Triangle Count", "%d", pMesh->pVertexLayout->indexCount / 3);
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