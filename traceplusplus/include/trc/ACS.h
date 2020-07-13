#pragma once

#include "Core.h"
#include "VertexLayout.h"
#include "ShaderLayout.h"

namespace ACS
{
  template<typename T>
  struct TInstance
  {
    inline static T* spInstance = nullptr;

    template<typename ... Args>
    inline static T* Act(Args&&... arg) { if (!spInstance) { spInstance = new T(std::forward<Args>(arg)...); } return spInstance; }
    inline static T* Act()              { if (!spInstance) { spInstance = new T; }                             return spInstance; }
  };

  struct TEvent
  {
    enum class TEventType : s32 { None = -1, Mouse, Keyboard, Window };

    TEventType eventType = TEventType::None;

    TEvent(TEventType eventType) : eventType(eventType) {}
  };
  struct TComponent
  {
    const s8* name = nullptr;

    TComponent(const s8* name) : name(name) {}

    virtual void Debug() {}
  };
  struct TActor
  {
    const s8* name = nullptr;

    TActor(const s8* name) : name(name) {}
  };

  namespace Components
  {
    struct TTransform : ACS::TComponent
    {
      r32v3 position      = {};
      r32v3 rotation      = {};
      r32v3 scale         = {};
      r32v3 right         = { 1.f, 0.f, 0.f };
      r32v3 up            = { 0.f, 1.f, 0.f };
      r32v3 forward       = { 0.f, 0.f, 1.f };
      r32v3 localRight    = right;
      r32v3 localUp       = up;
      r32v3 localForward  = forward;
      r32m4 modelMatrix   = glm::identity<r32m4>();

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
      r32m4       projectionMatrix = glm::identity<r32m4>();
      r32m4       viewMatrix       = glm::identity<r32m4>();

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
    struct TMaterial : ACS::TComponent
    {

    };
    // implicit required types are 'TVertexLayout' && 'TShaderLayout' && 'TTransform'
    // enable static_assert for missing components
    // check static order missmatch
    struct TLambertShader : ACS::TComponent
    {
      TTransform*    pTransform    = nullptr;
      TMesh*         pMesh         = nullptr;
      TShaderLayout* pShaderLayout = nullptr;
      //TRenderer*     pRenderer     = nullptr;

      TLambertShader(TActor* pActor, TShaderLayout* pShaderLayout);

      void Render() const;
    };
    struct TController : TComponent
    {
      TController(const s8* name) : TComponent(name) {}

      virtual void Update(const r32 deltaTime) {}
      virtual void Event(TEvent& event) {}
    };
  }
}