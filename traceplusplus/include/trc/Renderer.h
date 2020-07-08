#pragma once

#include "Core.h"

struct TRenderer
{
  struct TRenderJob
  {
    u32 program;
    u32 vao;
  };

  inline static std::queue<TRenderJob> sRenderJobs = {};
  inline static std::vector<TRenderJob> sLambertPass = {};

  static void Submit(const TRenderJob& renderJob) { sRenderJobs.emplace(renderJob); }
  static void Debug();
};