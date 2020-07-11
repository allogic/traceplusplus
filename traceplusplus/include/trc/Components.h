#pragma once

#include "Core.h"
#include "ACS.h"
#include "VertexLayout.h"
#include "ShaderLayout.h"

struct TTransform : ACS::TComponent
{
  r32v3 position = {};
  r32v3 rotation = {};
  r32v3 scale    = {};

  TTransform(
    const r32v3& position = { 0.f, 0.f, 0.f },
    const r32v3& rotation = { 0.f, 0.f, 0.f },
    const r32v3& scale    = { 1.f, 1.f, 1.f })
    : position(position)
    , rotation(rotation)
    , scale(scale)
    , TComponent("Transform") {}
};

struct TCamera : ACS::TComponent
{
  enum class TProjection : s32 { None = -1, Orthographic, Perspective };

  TProjection projection       = TProjection::None;
  r32         fov              = glm::radians(45.f);
  r32         near             = 0.001f;
  r32         far              = 10000.f;
  const r32v3 right            = { 1.f, 0.f, 0.f };
  const r32v3 up               = { 0.f, 1.f, 0.f };
  const r32v3 forward          = { 0.f, 0.f, 1.f };
  r32v3       localRight       = right;
  r32v3       localUp          = up;
  r32v3       localForward     = forward;
  r32         positionSpeed    = 7.0f;
  r32         rotationSpeed    = 1.5f;
  r32v2       rotationDrag     = {};
  r32         rotationDecay    = 15.0f;
  r32         rotationDeadzone = 0.001f;
  r32v2       rotationVelocity = {};
  r32m4       projectionTensor = glm::identity<r32m4>();
  r32m4       viewTensor       = glm::identity<r32m4>();

  TCamera(TProjection projection)
    : projection(projection)
    , TComponent("Camera") {}

  virtual void UpdateProjection(r32 aspect);
  virtual void UpdateView(TTransform* pTransform);
};

struct TMesh : ACS::TComponent
{
  TVertexLayout* pVertexLayout = nullptr;

  TMesh(TVertexLayout* pVertexLayout)
    : pVertexLayout(pVertexLayout)
    , TComponent("Mesh") {}
};

struct TShader : ACS::TComponent
{
  TShaderLayout* pShaderLayout = nullptr;

  TShader(TShaderLayout* pShaderLayout)
    : pShaderLayout(pShaderLayout)
    , TComponent("Shader") {}
};