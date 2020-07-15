#pragma once

#include "Core.h"
#include "StandardShaders.h"

namespace ShaderLayout
{
  enum class TShaderType : s32
  {
    None = -1,
    StandardShader,
    ComputeShader,
  };

  struct TShaderProgram
  {
    TShaderType type    = TShaderType::None;
    u32         program = 0;

    TShaderProgram(TShaderType type) : type(type) {}
    virtual ~TShaderProgram()
    {
      //glDeleteProgram(program);
    }
  };
  struct TLambertShader<TLambertVertexSource, TLambertVertexSource> : TShaderProgram
  {
    TStandardProgram()
      : TShaderProgram(TShaderType::StandardShader) {}
  };
  struct TTransformationProgram : TShaderProgram
  {
    TComputeShader()
      : TShaderProgram(TShaderType::ComputeShader) {}
  };

  static s32 CheckStatus(s32 shader, s32 type);
  static s32 LinkShader(u32 program, u32 shader);
  static s32 CompileShader(u32 shader, const s8* pSource);

  template<typename T, typename ... TSources>
  requires std::is_same_v<TShaderType, T>
  inline static s32 MakeProgram(TShaderProgram* pShader, TSources&& ... pSource)
  {

  }
}