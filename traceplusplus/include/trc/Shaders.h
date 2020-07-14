#pragma once

#include "Core.h"

constexpr static const s8 VertexTransformations[] = R"glsl(
#version 460 core

layout (std430, binding = 0) buffer bTransformation
{ 
  vec4 model;
};

void main()
{

}
)glsl";

constexpr static const s8 FragmentTransformations[] = R"glsl(
#version 460 core

void main()
{
  
}
)glsl";

constexpr static const s8 VertexLambert[] = R"glsl(
#version 460 core

layout (location = 0) in vec3 lPosition;
layout (location = 1) in vec2 lUv;

layout (std430, binding = 0) buffer bTransformation
{ 
  vec4 model;
};

uniform mat4  uProjection;
uniform mat4  uView;

void main()
{
  gl_Position = uProjection * uView * model * vec4(lPosition, 1.);
}
)glsl";

constexpr static const s8 FragmentLambert[] = R"glsl(
#version 460 core

out vec4 FragColor;

void main()
{
  FragColor = vec4(1., 1., 0., 1.);
}
)glsl";