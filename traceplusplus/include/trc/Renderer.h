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
  struct TRenderJobComp
  {
    auto operator ()(const TRenderJob& lhs, const TRenderJob& rhs) const
    {
      return lhs.pVertexLayout != rhs.pVertexLayout &&
             lhs.pShaderLayout != rhs.pShaderLayout;
    }
  };

  struct TTechnic
  {
    
  };
  struct TTransfomrationTechnic : TTechnic
  {
    ACS::Components::TTransform* pTransform = nullptr;

    TTransfomrationTechnic(
      ACS::Components::TTransform* pTransform)
      : TTechnic()
      , pTransform(pTransform) {}
  };
  struct TLambertTechnic : TTechnic
  {
    ACS::Components::TTransform* pTransform = nullptr;

    TLambertTechnic(
      ACS::Components::TTransform* pTransform)
      : TTechnic()
      , pTransform(pTransform) {}
  };

  std::set<TRenderJob, TRenderJobComp> transformationPass     = {};
  std::set<TRenderJob, TRenderJobComp> lambertPass            = {};
  std::vector<TTransfomrationTechnic>  transformationTechnics = {};
  std::vector<TLambertTechnic>         lambertTechnics        = {};

  template<typename T>
  requires std::is_base_of_v<TTechnic, T>
  inline void Submit(const TRenderJob& job, const T& technic)
  {
    if constexpr (std::is_same_v<TTransfomrationTechnic, T>)
    {
      transformationPass.emplace(job);
      transformationTechnics.emplace_back(technic);
    }
    else if constexpr (std::is_same_v<TLambertTechnic, T>)
    {
      lambertPass.emplace(job);
      lambertTechnics.emplace_back(technic);
    }
  }
  void Render(const ACS::Components::TCamera* pCamera);
  void Flush();
  void Debug();
};