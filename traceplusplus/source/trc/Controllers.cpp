#include <GLFW/glfw3.h>

#include "imgui.h"

#include "trc/Controllers.h"

TCameraController::TCameraController(ACS::TActor* pActor)
  : TComponent("Camera Controller")
{
  pTransform = ACS::Obtain<TTransform>(pActor);
  pCamera = ACS::Obtain<TCamera>(pActor);
}

void TCameraController::Update(const r32 deltaTime)
{
  this->deltaTime = deltaTime;

  if (glm::length(pCamera->rotationVelocity) > pCamera->rotationDeadzone)
    pCamera->rotationVelocity += -pCamera->rotationVelocity * pCamera->rotationDecay * deltaTime;
  else
    pCamera->rotationVelocity = { 0.f, 0.f };

  pCamera->rotationVelocity += pCamera->rotationDrag * pCamera->rotationSpeed * deltaTime;

  if (pCamera->rotationVelocity.x > 180.f) pCamera->rotationVelocity.x = -180.f;
  if (pCamera->rotationVelocity.x < -180.f) pCamera->rotationVelocity.x = 180.f;

  if (pCamera->rotationVelocity.y > 180.f) pCamera->rotationVelocity.y = -180.f;
  if (pCamera->rotationVelocity.y < -180.f) pCamera->rotationVelocity.y = 180.f;

  r32m4 localRotation = glm::identity<r32m4>();
  localRotation = glm::rotate(localRotation, glm::radians(pCamera->rotationVelocity.y), pCamera->localRight);
  localRotation = glm::rotate(localRotation, glm::radians(pCamera->rotationVelocity.x), pCamera->localUp);

  pCamera->localRight = localRotation * r32v4{ pCamera->localRight, 1.f };
  pCamera->localUp = localRotation * r32v4{ pCamera->localUp, 1.f };
  pCamera->localForward = localRotation * r32v4{ pCamera->localForward, 1.f };

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
    auto keyEvent = reinterpret_cast<TKeyEvent&>(event);

    if (keyEvent.key == GLFW_KEY_W && (keyEvent.action == GLFW_PRESS || keyEvent.action == GLFW_REPEAT))
      pTransform->position += pCamera->localForward * pCamera->positionSpeed * deltaTime;
    if (keyEvent.key == GLFW_KEY_S && (keyEvent.action == GLFW_PRESS || keyEvent.action == GLFW_REPEAT))
      pTransform->position -= pCamera->localForward * pCamera->positionSpeed * deltaTime;

    if (keyEvent.key == GLFW_KEY_A && (keyEvent.action == GLFW_PRESS || keyEvent.action == GLFW_REPEAT))
      pTransform->position += pCamera->localRight * pCamera->positionSpeed * deltaTime;
    if (keyEvent.key == GLFW_KEY_D && (keyEvent.action == GLFW_PRESS || keyEvent.action == GLFW_REPEAT))
      pTransform->position -= pCamera->localRight * pCamera->positionSpeed * deltaTime;
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
      pCamera->rotationDrag = { 0.f, 0.f };
    }
  }
  break;
  }
}

void TCameraController::Debug()
{
  ImGui::SliderFloat("Position Speed", &pCamera->positionSpeed, 0.f, 10.f);
  ImGui::SliderFloat("Rotation Speed", &pCamera->rotationSpeed, 0.f, 10.f);
  ImGui::LabelText("Rotation Drag", "{%3.3f, %3.3f}", pCamera->rotationDrag.x, pCamera->rotationDrag.y);
  ImGui::SliderFloat("Rotation Decay", &pCamera->rotationDecay, 0.f, 10.f);
  ImGui::SliderFloat("Rotation Deadzone", &pCamera->rotationDeadzone, 0.f, 1.f);
  ImGui::LabelText("Rotation Velocity", "{%3.3f, %3.3f}", &pCamera->rotationVelocity.x, &pCamera->rotationVelocity.y);
}

TRenderController::TRenderController(ACS::TActor* pActor)
  : TComponent("Render Controller")
{
  pMesh = ACS::Obtain<TMesh>(pActor);
  pTransform = ACS::Obtain<TTransform>(pActor);
  pShader = ACS::Obtain<TShader>(pActor);

  randRotVel = r32v3{ 1 + std::rand() % 5, 1 + std::rand() % 5, 1 + std::rand() % 5 };
}

void TRenderController::Update(const r32 deltaTime)
{
  pTransform->rotation += randRotVel * deltaTime;
}

void TRenderController::Render() const
{
  auto job = Renderer::TRenderJob{ pMesh->pVertexLayout, pShader->pShaderLayout };
  auto technic = Renderer::TLambertTechnic{ pTransform };

  Renderer::Submit(job, technic);
}