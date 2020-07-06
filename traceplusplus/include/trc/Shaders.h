#pragma once

#include "Core.h"

constexpr static const s8 VertexDefault[] = R"glsl(
#version 460 core

layout (location = 0) in vec3 lPosition;
layout (location = 1) in vec2 lUv;

in vec2 iScreenSize;

uniform mat4 uProjection;
uniform mat4 uView;
uniform mat4 uModel;

out vec2 oUv;
out vec2 oScreenSize;

void main()
{
  oUv = lUv;
  oScreenSize = iScreenSize;

  gl_Position = uProjection * uView * uModel * vec4(lPosition, 1.);
}
)glsl";

constexpr static const s8 FragmentDefault[] = R"glsl(
#version 460 core

in vec2 oUv;
in vec2 oScreenSize;

out vec4 FragColor;

void main()
{
  FragColor = vec4(1., 0., 0., 1.);
}
)glsl";