#include "glad/glad.h"

#include "trc/ShaderLayout.h"

s32 ShaderLayout::CheckStatus(s32 shader, s32 type)
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

s32 ShaderLayout::LinkShader(u32 program, u32 shader)
{
  glAttachShader(program, shader);

  glLinkProgram(program);

  if (!CheckStatus(program, GL_LINK_STATUS))
    return 0;

  return 1;
}

s32 ShaderLayout::CompileShader(u32 shader, const s8* pSource)
{
  glShaderSource(shader, 1, &pSource, nullptr);
  glCompileShader(shader);

  if (!CheckStatus(shader, GL_COMPILE_STATUS))
    return 0;

  return 1;
}

//s32 ShaderLayout::MakeProgram()
//{
//  u32 program = glCreateProgram();
//
//  if (!program) return 0;
//
//  switch (T)
//  {
//  case TShaderType::StandardShader:
//    u32 vertexShader = glCreateShader(GL_VERTEX_SHADER);
//    u32 fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//
//    if (!(
//      vertexShader &&
//      fragmentShader))
//    {
//      glDeleteProgram(program);
//      return 0;
//    }
//
//    //if (!(
//    //  CompileShader(vertexShader, pVertexSource) &&
//    //  CompileShader(fragmentShader, pFragmentShader) &&
//    //  LinkShader(program, vertexShader) &&
//    //  LinkShader(program, fragmentShader)))
//    //{
//    //  glDeleteShader(vertexShader);
//    //  glDeleteShader(fragmentShader);
//    //  glDeleteProgram(program);
//    //  return 0;
//    //}
//    break;
//  case TShaderType::ComputeShader:
//    //u32 vertexShader = glCreateShader(GL_COMPUTE_SHADER);
//    break;
//  }
//
//  return 1;
//}