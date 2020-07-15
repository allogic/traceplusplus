#pragma once

#include "Core.h"
#include "ACS.h"

struct TRenderer : ACS::TInstance<TRenderer>
{
  struct TRenderJob
  {
    TVertexLayout*                pVertexLayout = nullptr;
    ShaderLayout::TShaderProgram* pShader       = nullptr;

    TRenderJob(
      TVertexLayout* pVertexLayout,
      ShaderLayout::TShaderProgram* pShader)
      : pVertexLayout(pVertexLayout)
      , pShader(pShader) {}
    virtual ~TRenderJob() = default;
  };
  struct TRenderJobComp
  {
    inline auto operator ()(const TRenderJob& lhs, const TRenderJob& rhs) const
    {
      return lhs.pVertexLayout != rhs.pVertexLayout && lhs.pShader != rhs.pShader;
    }
  };

  struct TTechnic
  {
    
  };
  struct TLambertTechnic : TTechnic
  {
    ACS::Components::TTransform* pTransform = nullptr;

    TLambertTechnic(
      ACS::Components::TTransform* pTransform)
      : TTechnic()
      , pTransform(pTransform) {}
  };

  std::set<TRenderJob, TRenderJobComp> lambertPass            = {};
  std::vector<TLambertTechnic>         lambertTechnics        = {};

  template<typename T>
  requires std::is_base_of_v<TTechnic, T>
  inline void Submit(const TRenderJob& job, const T& technic)
  {
    if constexpr (std::is_same_v<TLambertTechnic, T>)
    {
      lambertPass.emplace(job);
      lambertTechnics.emplace_back(technic);
    }
  }
  void Render(const ACS::Components::TCamera* pCamera);
  void Flush();
  void Debug();
};