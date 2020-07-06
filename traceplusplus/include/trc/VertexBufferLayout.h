#pragma once

#include "Core.h"

struct TVertexBufferLayout
{
  const u32  width          = 0;
  const u32  height         = 0;
  const u32  vertexCount    = 0;
  const u32  indexCount     = 0;
  const u32  vertexLocation = 0;
  const u32  indexLocation  = 1;
  u32        vao            = 0;
  u32        pVbos[2]       = {};
  r32*       pVertices      = nullptr;
  u32*       pIndices       = nullptr;

  TVertexBufferLayout(u32 width = 0, u32 height = 0, u32 indexCount = 0);
  virtual ~TVertexBufferLayout();

  s32 CreateBuffers();
};