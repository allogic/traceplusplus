#include "glad/glad.h"

#include "trc/VertexLayout.h"

TVertexLayout::TVertexLayout(u32 vertexCount, u32 indexCount)
  : vertexCount(vertexCount)
  , indexCount(indexCount)
{
  pVertices = new r32[vertexCount];
  pIndices = new u32[indexCount];
}

TVertexLayout::~TVertexLayout()
{
  glDeleteBuffers(2, &pVbos[0]);
  glDeleteVertexArrays(1, &vao);

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
  //glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, 0);
  //glVertexAttribPointer(1, 2, GL_FLOAT, 0, 2 * sizeof(r32), (void*)(3 * sizeof(r32)));
  glVertexAttribPointer(0, 3, GL_FLOAT, 0, 5 * sizeof(r32), 0);
  glVertexAttribPointer(1, 2, GL_FLOAT, 0, 5 * sizeof(r32), (void*)(3 * sizeof(r32)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glBindBuffer(0, 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pVbos[1]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(u32), pIndices, GL_STATIC_DRAW);
  glBindBuffer(0, 0);

  glBindVertexArray(0);

  return 1;
}