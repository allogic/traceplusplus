#vertex

layout (location = 0) in vec3 lPosition;

in vec2 iScreenSize;

uniform mat4 uProjection;
uniform mat4 uView;
uniform mat4 uModel;

out vec2 oScreenSize;

void main()
{
  oScreenSize = iScreenSize;

  gl_Position = uProjection * uView * uModel * vec4(lPosition, 1.);
}

#fragment

in vec2 iScreenSize;

out vec4 oFragColor;

void main()
{
  oFragColor = vec4(iScreenSize, 0., 1.);
}