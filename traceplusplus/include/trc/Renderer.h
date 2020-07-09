#pragma once

#include "Core.h"
#include "VertexLayout.h"
#include "ShaderLayout.h"

struct TRenderer
{
  struct TRenderJob
  {
    TVertexLayout* pVertexLayout = nullptr;
    TShaderLayout* pShaderLayout = nullptr;

    TRenderJob(
      TVertexLayout* pVertexLayout,
      TShaderLayout* pShaderLayout)
      : pVertexLayout(pVertexLayout)
      , pShaderLayout(pShaderLayout) {}
    virtual ~TRenderJob() = default;
  };

  struct TLambertJobComp
  {
    auto operator ()(const TRenderJob& lhs, const TRenderJob& rhs) const
    {
      return lhs.pVertexLayout != rhs.pVertexLayout &&
             lhs.pShaderLayout != rhs.pShaderLayout;
    }
  };

  inline static std::set<TRenderJob, TLambertJobComp> sLambertPass = {};

  static void Submit(const TRenderJob& renderJob) { sLambertPass.emplace(renderJob); }
  static void Render(const r32 deltaTime);
  static void Clear();
  static void Debug();
};