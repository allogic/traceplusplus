#pragma once

#include "Core.h"

struct TVertexLayout
{
  const u32  vertexCount    = 0;
  const u32  indexCount     = 0;
  const u32  vertexLocation = 0;
  const u32  indexLocation  = 1;
  u32        vao            = 0;
  u32        pVbos[2]       = {};
  r32*       pVertices      = nullptr;
  u32*       pIndices       = nullptr;

  TVertexLayout(u32 vertexCount, u32 indexCount);
  virtual ~TVertexLayout();

  s32 CreateBuffers();
};