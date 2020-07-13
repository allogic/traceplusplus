#include <GLFW/glfw3.h>

#include "imgui.h"

#include "trc/Scene.h"
#include "trc/Controllers.h"

TCameraController::TCameraController(ACS::TActor* pActor)
  : ACS::Components::TController("Camera Controller")
  , pTransform(TScene::Act()->Obtain<ACS::Components::TTransform>(pActor))
  , pCamera(TScene::Act()->Obtain<ACS::Components::TCamera>(pActor))
{

}

void TCameraController::Update(const r32 deltaTime)
{
  this->deltaTime = deltaTime;

  if (glm::length(rotationVelocity) > rotationDeadzone)
    rotationVelocity += -rotationVelocity * rotationDecay * deltaTime;
  else
    rotationVelocity = { 0.f, 0.f };

  rotationVelocity += rotationDrag * rotationSpeed * deltaTime;

  if (rotationVelocity.x > 180.f) rotationVelocity.x = -180.f;
  if (rotationVelocity.x < -180.f) rotationVelocity.x = 180.f;

  if (rotationVelocity.y > 180.f) rotationVelocity.y = -180.f;
  if (rotationVelocity.y < -180.f) rotationVelocity.y = 180.f;

  r32m4 localRotation = glm::identity<r32m4>();
  localRotation = glm::rotate(localRotation, glm::radians(rotationVelocity.y), pTransform->localRight);
  localRotation = glm::rotate(localRotation, glm::radians(rotationVelocity.x), pTransform->localUp);

  pTransform->localRight = localRotation * r32v4{ pTransform->localRight, 1.f };
  pTransform->localUp = localRotation * r32v4{ pTransform->localUp, 1.f };
  pTransform->localForward = localRotation * r32v4{ pTransform->localForward, 1.f };

  // TODO: 'Renderer::Camera' or 'ACS::Obtain<TCamera>'
  // TODO: even better turn everything into a component

  pCamera->UpdateProjection(1280.f / 720.f);
  pCamera->UpdateView(pTransform);
}

void TCameraController::Event(ACS::TEvent& event)
{
  switch (event.eventType)
  {
  case ACS::TEvent::TEventType::Keyboard:
  {
    auto keyEvent = (TKeyEvent&)(event);

    if (keyEvent.key == GLFW_KEY_W && (keyEvent.action == GLFW_PRESS || keyEvent.action == GLFW_REPEAT))
      pTransform->position += pTransform->localForward * positionSpeed * deltaTime;
    if (keyEvent.key == GLFW_KEY_S && (keyEvent.action == GLFW_PRESS || keyEvent.action == GLFW_REPEAT))
      pTransform->position -= pTransform->localForward * positionSpeed * deltaTime;

    if (keyEvent.key == GLFW_KEY_A && (keyEvent.action == GLFW_PRESS || keyEvent.action == GLFW_REPEAT))
      pTransform->position += pTransform->localRight * positionSpeed * deltaTime;
    if (keyEvent.key == GLFW_KEY_D && (keyEvent.action == GLFW_PRESS || keyEvent.action == GLFW_REPEAT))
      pTransform->position -= pTransform->localRight * positionSpeed * deltaTime;
  }
  break;
  case ACS::TEvent::TEventType::Mouse:
  {
    auto mouseEvent = reinterpret_cast<TMouseButtonEvent&>(event);

    if (mouseEvent.button == GLFW_MOUSE_BUTTON_RIGHT && (mouseEvent.action == GLFW_PRESS || mouseEvent.action == GLFW_REPEAT))
    {
      // TODO: Make mouse non global static

      //pCamera->rotationDrag.x = sMouse.invertX ? -sMouse.deltaPosition.x : sMouse.deltaPosition.x; // aspect
      //pCamera->rotationDrag.y = sMouse.invertY ? -sMouse.deltaPosition.y : sMouse.deltaPosition.y;
    }
    else
    {
      rotationDrag = { 0.f, 0.f };
    }
  }
  break;
  }
}

void TCameraController::Debug()
{
  ImGui::SliderFloat("Position Speed", &positionSpeed, 0.f, 10.f);
  ImGui::SliderFloat("Rotation Speed", &rotationSpeed, 0.f, 10.f);
  ImGui::LabelText("Rotation Drag", "{%3.3f, %3.3f}", rotationDrag.x, rotationDrag.y);
  ImGui::SliderFloat("Rotation Decay", &rotationDecay, 0.f, 10.f);
  ImGui::SliderFloat("Rotation Deadzone", &rotationDeadzone, 0.f, 1.f);
  ImGui::LabelText("Rotation Velocity", "{%3.3f, %3.3f}", &rotationVelocity.x, &rotationVelocity.y);
}

TCubeController::TCubeController(ACS::TActor* pActor)
  : ACS::Components::TController("Cube Controller")
  , pMesh(TScene::Act()->Obtain<ACS::Components::TMesh>(pActor))
  , pTransform(TScene::Act()->Obtain<ACS::Components::TTransform>(pActor))
{
  randRotVel = r32v3{ 1 + std::rand() % 5, 1 + std::rand() % 5, 1 + std::rand() % 5 };
}

void TCubeController::Update(const r32 deltaTime)
{
  pTransform->rotation += randRotVel * deltaTime;
}