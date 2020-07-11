#pragma once

#include "Core.h"
#include "Components.h"
#include "VertexLayout.h"
#include "ShaderLayout.h"

class Renderer
{
public:
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
  struct TLambertTechnic
  {
    TTransform* pTransform = nullptr;
  };

private:
  struct TLambertJobComp
  {
    auto operator ()(const TRenderJob& lhs, const TRenderJob& rhs) const
    {
      return lhs.pVertexLayout != rhs.pVertexLayout &&
             lhs.pShaderLayout != rhs.pShaderLayout;
    }
  };

  inline static std::set<TRenderJob, TLambertJobComp> sLambertPass     = {};
  inline static std::vector<TLambertTechnic>          sLambertTechnics = {};

public:
  static void Submit(const TRenderJob& job, const TLambertTechnic& technic);
  static void Render(const TCamera* pCamera);
  static void Flush();
  static void Debug();
};