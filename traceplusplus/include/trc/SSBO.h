#pragma once

#include "Core.h"

struct SSBO
{
  u32  ssbo        = 0;
  u32  bufferSize  = 0;
  r32* pBuffer     = nullptr;

  SSBO(u32 bufferSize);
  virtual ~SSBO();

  s32  CreateBuffer();
  void Copy();
};