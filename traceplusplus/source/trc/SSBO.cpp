#include "glad/glad.h"

#include "trc/SSBO.h"

SSBO::SSBO(u32 bufferSize)
  : bufferSize(bufferSize)
{
  pBuffer = new r32[bufferSize];
}

SSBO::~SSBO()
{
  glDeleteBuffers(1, &ssbo);

  delete[] pBuffer;
}

s32 SSBO::CreateBuffer()
{
  glGenBuffers(1, &ssbo);

  if (!ssbo)
    return 0;

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);

  glBufferData(GL_SHADER_STORAGE_BUFFER, bufferSize * sizeof(r32), pBuffer, GL_DYNAMIC_COPY);

  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

  return 1;
}

void SSBO::Copy()
{
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);

  void* pDest = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);

  std::memcpy(pDest, pBuffer, bufferSize * sizeof(r32));

  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
}