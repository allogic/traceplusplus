#include "trc/ShaderLayout.h"

#include "glad/glad.h"

s32 TShaderLayout::CheckStatus(s32 shader, s32 type) const
{
  s32 compileInfo = 0;
  s32 compileInfoSize = 0;

  glGetShaderiv(shader, type, &compileInfo);
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &compileInfoSize);

  std::string log = "";

  if (compileInfoSize > 0)
  {
    log.resize(compileInfoSize);

    glGetShaderInfoLog(shader, compileInfoSize, nullptr, &log[0]);

    std::printf(log.c_str());

    return 0;
  }

  return 1;
}

s32 TShaderLayout::LinkShaders() const
{
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);

  glLinkProgram(program);

  if (!CheckStatus(program, GL_LINK_STATUS))
    return 0;

  return 1;
}

s32 TShaderLayout::CompileShader(u32 shader, const s8* pSource) const
{
  glShaderSource(shader, 1, &pSource, nullptr);
  glCompileShader(shader);

  if (!CheckStatus(shader, GL_COMPILE_STATUS))
    return 0;

  return 1;
}

s32 TShaderLayout::CompileShaders(const s8* pVertexSource, const s8* pFragmentSource)
{
  vertexShader = glCreateShader(GL_VERTEX_SHADER);

  if (!CompileShader(vertexShader, pVertexSource))
    return 0;

  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

  if (!CompileShader(fragmentShader, pFragmentSource))
    return 0;

  program = glCreateProgram();

  if (!LinkShaders())
    return 0;

  return 1;
}