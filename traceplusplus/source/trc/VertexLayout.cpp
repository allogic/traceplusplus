#include "glad/glad.h"

#include "trc/VertexLayout.h"

TVertexLayout::TVertexLayout(u32 width, u32 height, u32 indexCount)
  : width(width)
  , height(height)
  , vertexCount(width* height)
  , indexCount(indexCount)
{
  pVertices = (r32*)std::calloc(vertexCount, sizeof(r32));
  pIndices = (u32*)std::calloc(indexCount, sizeof(u32));
}

TVertexLayout::~TVertexLayout()
{
  delete[] pVertices;
  delete[] pIndices;
}

s32 TVertexLayout::CreateBuffers()
{
  glGenVertexArrays(1, &vao);

  if (!vao)
    return 0;

  glBindVertexArray(vao);

  glGenBuffers(2, &pVbos[0]);

  if (!pVbos[0] || !pVbos[1])
    return 0;

  glBindBuffer(GL_ARRAY_BUFFER, pVbos[0]);
  glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(r32), pVertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, 0, 3 * sizeof(r32), 0);
  glVertexAttribPointer(1, 2, GL_FLOAT, 0, 2 * sizeof(r32), 0);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glBindBuffer(0, 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pVbos[1]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(u32), pIndices, GL_STATIC_DRAW);
  glBindBuffer(0, 0);

  glBindVertexArray(0);

  return 1;
}