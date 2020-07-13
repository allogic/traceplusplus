#pragma once

#include "Core.h"
#include "ACS.h"
#include "Events.h"

struct TCameraController : ACS::Components::TController
{
  ACS::Components::TTransform* pTransform       = nullptr;
  ACS::Components::TCamera*    pCamera          = nullptr;

  r32                          deltaTime        = 0.f;
  r32                          positionSpeed    = 7.0f;
  r32                          rotationSpeed    = 1.5f;
  r32v2                        rotationDrag     = {};
  r32                          rotationDecay    = 15.0f;
  r32                          rotationDeadzone = 0.001f;
  r32v2                        rotationVelocity = {};

  TCameraController(ACS::TActor* pActor);

  void Update(const r32 deltaTime) override;
  void Event(ACS::TEvent& event) override;
  void Debug() override;
};

struct TCubeController : ACS::Components::TController
{
  ACS::Components::TTransform* pTransform = nullptr;
  ACS::Components::TMesh*      pMesh      = nullptr;

  r32v3                        randRotVel = {};

  TCubeController(ACS::TActor* pActor);

  void Update(const r32 deltaTime) override;
};