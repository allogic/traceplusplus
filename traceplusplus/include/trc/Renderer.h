#pragma once

#include "Core.h"
#include "ACS.h"

struct TRenderer : ACS::TInstance<TRenderer>
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
  struct TLambertTechnic
  {
    ACS::Components::TTransform* pTransform = nullptr;
  };
  struct TLambertJobComp
  {
    auto operator ()(const TRenderJob& lhs, const TRenderJob& rhs) const
    {
      return lhs.pVertexLayout != rhs.pVertexLayout &&
        lhs.pShaderLayout != rhs.pShaderLayout;
    }
  };

  std::set<TRenderJob, TLambertJobComp> lambertPass     = {};
  std::vector<TLambertTechnic>          lambertTechnics = {};

  void Submit(const TRenderJob& job, const TLambertTechnic& technic);
  void Render(const ACS::Components::TCamera* pCamera);
  void Flush();
  void Debug();
};