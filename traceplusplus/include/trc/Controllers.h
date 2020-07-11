#pragma once

#include "Core.h"
#include "ACS.h"
#include "Components.h"
#include "Events.h"
#include "Renderer.h"

struct TCameraController : ACS::TComponent
{
  TTransform* pTransform = nullptr;
  TCamera*    pCamera    = nullptr;
  r32         deltaTime  = 0.f;

  TCameraController(ACS::TActor* pActor);

  void Update(const r32 deltaTime) override;
  void Event(ACS::TEvent& event) override;
  void Debug() override;
};

struct TRenderController : ACS::TComponent
{
  TTransform* pTransform = nullptr;
  TMesh*      pMesh      = nullptr;
  TShader*    pShader    = nullptr;
  r32v3       randRotVel = {};

  TRenderController(ACS::TActor* pActor);

  void Update(const r32 deltaTime) override;
  void Render() const override;
};