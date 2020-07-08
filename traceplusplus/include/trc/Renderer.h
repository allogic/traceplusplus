#pragma once

#include "Core.h"

struct TRenderer
{
  struct TRenderJob
  {
    u32 program;
    u32 vao;
  };

  inline static std::vector<TRenderJob> sLambertPass = {};

  static void Submit(const TRenderJob& renderJob) { sLambertPass.emplace_back(renderJob); }
  static void Render(const r32 deltaTime);
  static void Clear();
  static void Debug();
};