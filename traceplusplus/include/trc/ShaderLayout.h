#pragma once

#include "Core.h"

struct TShaderLayout
{
  u32 program        = 0;
  u32 vertexShader   = 0;
  u32 fragmentShader = 0;

  TShaderLayout() = default;
  virtual ~TShaderLayout() = default;

  s32 CheckStatus(s32 shader, s32 type) const;
  s32 LinkShaders() const;
  s32 CompileShader(u32 shader, const s8* pSource) const;
  s32 CompileShaders(const s8* pVertexSource, const s8* pFragmentSource);
};