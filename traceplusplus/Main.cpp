#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

typedef char           s8;
typedef unsigned char  u8;

typedef int           s32;
typedef unsigned int  u32;
typedef float         f32;

typedef glm::fvec2    fv2;
typedef glm::fvec3    fv3;

typedef glm::fmat4    fm4;

struct TTransform
{
  fv3 position        = {};
  fv3 rotation        = {};
  fv3 scale           = { 1.f, 1.f, 1.f };

  fm4 positionTensor  = {};
  fm4 rotationTensor  = {};
  fm4 scaleTensor     = {};
};

template<u32 Width, u32 Height, u32 AttrCount, u32 IndexCount>
struct TVertexLayout
{
  constexpr static const u32   Width = Width;
  constexpr static const u32   Height = Height;

  constexpr static const u32   AttrCount = AttrCount;
  constexpr static const u32   VertexCount = Width * Height;
  constexpr static const u32   IndexCount = IndexCount;

  constexpr static const u32   VertexLocation = 0;
  constexpr static const u32   IndexLocation = 1;

  TVertexLayout()
  {
    pVertices = (f32*)std::calloc(VertexCount, sizeof(f32*));
    pIndices = (u32*)std::calloc(IndexCount, sizeof(u32));
  }

  u32                          vao              = 0;
  u32                          pVbos[AttrCount] = { 0, 0 };
  f32*                         pVertices        = nullptr;
  u32*                         pIndices         = nullptr;
  TTransform                   transform        = {};
};

struct TCamera
{
  TTransform transform = {};
};

#include <string>
#include <fstream>
#include <sstream>

#define GL_MAJOR 4
#define GL_MINOR 0

#define _STR(V) #V
#define STR(V) _STR(V)

#define IMGUI_GL_VERSION "#version " STR(GL_MAJOR) STR(GL_MINOR) "0 core"

#define FAILED_GLFW_INIT        -1
#define FAILED_GLFW_WINDOW      -2
#define FAILED_GL_CONTEXT       -3
#define FAILED_IMGUI_VERSION    -4
#define FAILED_IMGUI_CONTEXT    -5
#define FAILED_IMGUI_IMPL_GLFW  -6
#define FAILED_IMGUI_IMPL_GL    -7
#define FAILED_LOADING_SHADER   -8
#define FAILED_SHADER_UNIFORM   -9
#define FAILED_CREATING_BUFFER -10

//class CTracer
//  : public olc::PixelGameEngine
//{
//public:
//  CTracer() { sAppName = "Tracer"; }
//
//public:
//  b8 OnUserCreate() override
//  {
//    //scene.AddObject(&cube);
//    mScene.AddObject(&mSphere);
//
//    return 1;
//  }
//
//  b8 OnUserUpdate(f32 elapsedTime) override
//  {
//    Clear({ 0, 0, 0, 255 });
//
//    const fv2 screenSize = { ScreenWidth(), ScreenHeight() };
//
//    fv2 screenUvNorm;
//    fv3 rayOrig;
//    fv3 hitNormal;
//
//    olc::Pixel color;
//
//    mCamera.Update();
//
//    for (f32 x = 0.f; x < screenSize.x; x++)
//      for (f32 y = 0.f; y < screenSize.y; y++)
//      {
//        screenUvNorm = CCamera::NDC({ x, y }, screenSize);
//        rayOrig = mCamera.ProjectToWorld(screenUvNorm);
//
//        if (mCamera.Trace(screenUvNorm, 32, mScene, hitNormal))
//        {
//          const auto r = (uint8_t)glm::floor(hitNormal.r * 255.f);
//          const auto g = (uint8_t)glm::floor(hitNormal.g * 255.f);
//          const auto b = (uint8_t)glm::floor(hitNormal.b * 255.f);
//
//          Draw((s32)x, (s32)y, { r, g, b, 255 });
//        }
//      }
//
//    return 1;
//  }
//
//private:
//  fv3 mCameraRotation{ 0, 0, 0 };
//
//  CCamera mCamera{ { 0, 0, -1 }, { 0, 0, 1 }, { 0, 1, 0 } };
//
//  CScene mScene;
//
//  CCube mCube{ { 0, 0, 5 }, { 5, 5, 5 } };
//  CSphere mSphere{ { 0, 0, 0 }, 0.5f };
//};

constexpr static const s8 TraceVertexSource[] = R"glsl(
layout (location = 0) in vec3 lPosition;
layout (location = 1) in vec2 lUv;

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
)glsl";

constexpr static const s8 TraceFragmentSource[] = R"glsl(
in vec2 iScreenSize;

out vec4 FragColor;

void main()
{
  FragColor = vec4(1., 0., 0., 1.);
}
)glsl";

static s32 CheckStatus(s32 shader, s32 type, std::string& log)
{
  s32 compileInfo;
  s32 compileInfoSize;

  glGetShaderiv(shader, type, &compileInfo);
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &compileInfoSize);

  if (compileInfoSize > 0)
  {
    log.resize(compileInfoSize);

    glGetShaderInfoLog(shader, compileInfoSize, nullptr, &log[0]);

    return 0;
  }

  return 1;
}
static s32 LinkShaders(u32 program, u32 vertexShader, u32 fragmentShader)
{
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);

  glLinkProgram(program);

  std::string log;

  if (!CheckStatus(program, GL_LINK_STATUS, log))
  {
    std::printf(log.c_str());

    return 0;
  }

  return 1;
}
static s32 CompileShader(u32 shader, const std::string& source)
{
  std::string finalSource = std::string{ IMGUI_GL_VERSION "\n" + source };
  const char* pSource = finalSource.c_str();

  glShaderSource(shader, 1, &pSource, nullptr);
  glCompileShader(shader);

  std::string log;

  if (!CheckStatus(shader, GL_COMPILE_STATUS, log))
  {
    std::printf(log.c_str());

    return 0;
  }

  return 1;
}
static s32 CompileShaders(u32& program, const std::string& vertexSource, const std::string& fragmentSource)
{
  s32 vertexShader = glCreateShader(GL_VERTEX_SHADER);

  if (!CompileShader(vertexShader, vertexSource))
    return 0;

  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

  if (!CompileShader(fragmentShader, fragmentSource))
    return 0;

  program = glCreateProgram();

  if (!LinkShaders(program, vertexShader, fragmentShader))
    return 0;

  return 1;
}

template<u32 Width, u32 Height, u32 AttrCount, u32 IndexCount>
static s32 CreateBuffers(TVertexLayout<Width, Height, AttrCount, IndexCount>& layout)
{
  typedef TVertexLayout<Width, Height, AttrCount, IndexCount> T;

  glGenVertexArrays(1, &layout.vao);

  if (!layout.vao)
    return 0;

  glBindVertexArray(layout.vao);

  glGenBuffers(T::AttrCount, &layout.pVbos[0]);

  if (!layout.pVbos[T::VertexLocation] || !layout.pVbos[T::IndexLocation])
    return 0;

  glBindBuffer(GL_ARRAY_BUFFER, layout.pVbos[T::VertexLocation]);
  glBufferData(GL_ARRAY_BUFFER, T::VertexCount * sizeof(f32), layout.pVertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, 0);
  glVertexAttribPointer(1, 2, GL_FLOAT, 0, 3 * sizeof(f32), 0);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glBindBuffer(0, 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, layout.pVbos[T::IndexLocation]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, T::IndexCount * sizeof(u32), layout.pIndices, GL_STATIC_DRAW);
  glBindBuffer(0, 0);

  glBindVertexArray(0);

  return 1;
}

static s32 sStatus = 1;
static fv2 sScreenSize = { 1280, 720 };

s32 main()
{
  GLFWwindow* pWindow = nullptr;

  if (!glfwInit())
    return FAILED_GLFW_INIT;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GL_MAJOR);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GL_MINOR);
  glfwWindowHint(GLFW_SAMPLES, 0);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, 1);

  if (pWindow = glfwCreateWindow((s32)sScreenSize.x, (s32)sScreenSize.y, "Tracer", nullptr, nullptr), !pWindow)
    return FAILED_GLFW_WINDOW;

  glfwMakeContextCurrent(pWindow);

  glfwSetWindowCloseCallback(pWindow, [](GLFWwindow*) { sStatus = 0; });
  glfwSetWindowSizeCallback(pWindow, [](GLFWwindow*, s32 width, s32 height) { sScreenSize = { width, height }; glViewport(0, 0, (s32)sScreenSize.x, (s32)sScreenSize.y); });

  glfwSwapInterval(1);

  if (!gladLoadGL())
    return FAILED_GL_CONTEXT;

  if (!IMGUI_CHECKVERSION())
    return FAILED_IMGUI_VERSION;

  if (!ImGui::CreateContext())
    return FAILED_IMGUI_CONTEXT;

  ImGuiIO& io = ImGui::GetIO();
  ImGuiStyle& style = ImGui::GetStyle();
  style.WindowRounding = 0.f;
  style.WindowBorderSize = 0.f;

  if (!ImGui_ImplGlfw_InitForOpenGL(pWindow, 1))
    return FAILED_IMGUI_IMPL_GLFW;

  if (!ImGui_ImplOpenGL3_Init(IMGUI_GL_VERSION))
    return FAILED_IMGUI_IMPL_GL;

  TVertexLayout<4, 5, 2, 6> quadLayout;
  quadLayout.pVertices[0] = -1.f;
  quadLayout.pVertices[1] = -1.f;
  quadLayout.pVertices[2] =  0.f;
  quadLayout.pVertices[3] =  0.f;
  quadLayout.pVertices[4] =  0.f;

  quadLayout.pVertices[5] =  1.f;
  quadLayout.pVertices[6] = -1.f;
  quadLayout.pVertices[7] =  0.f;
  quadLayout.pVertices[8] =  1.f;
  quadLayout.pVertices[9] =  0.f;

  quadLayout.pVertices[10] = -1.f;
  quadLayout.pVertices[11] =  1.f;
  quadLayout.pVertices[12] =  0.f;
  quadLayout.pVertices[13] =  0.f;
  quadLayout.pVertices[14] =  1.f;

  quadLayout.pVertices[15] =  1.f;
  quadLayout.pVertices[16] =  1.f;
  quadLayout.pVertices[17] =  0.f;
  quadLayout.pVertices[18] =  1.f;
  quadLayout.pVertices[19] =  1.f;

  quadLayout.pIndices[0] = 3;
  quadLayout.pIndices[1] = 1;
  quadLayout.pIndices[2] = 2;
  quadLayout.pIndices[3] = 3;
  quadLayout.pIndices[4] = 2;
  quadLayout.pIndices[5] = 0;

  if (!CreateBuffers(quadLayout))
    return FAILED_CREATING_BUFFER;

  u32 shader = 0;

  if (!CompileShaders(shader, TraceVertexSource, TraceFragmentSource))
    return FAILED_LOADING_SHADER;

  u32 uProjection;
  u32 uView;
  u32 uModel;

  if (uProjection = glGetUniformLocation(shader, "uProjection"); uProjection < 0)
    return FAILED_SHADER_UNIFORM;

  if (uView = glGetUniformLocation(shader, "uView"); uView < 0)
    return FAILED_SHADER_UNIFORM;

  if (uModel = glGetUniformLocation(shader, "uModel"); uModel < 0)
    return FAILED_SHADER_UNIFORM;

  f32 time = 0.f;
  f32 prevTime = 0.f;
  f32 deltaTime = 0.f;

  s32 fps = 60;
  f32 renderRate = 1.f / fps;
  f32 prevRenderTime = 0.f;

  fv3 cameraPosition = { 0.f, 0.f, -10.f };

  fm4 projection;
  fm4 view;
  fm4 model;

  while (sStatus > 0)
  {
    glfwPollEvents();

    time = (f32)glfwGetTime();
    deltaTime = time - prevTime;
    renderRate = 1.f / fps;

    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shader);

    glBindVertexArray(quadLayout.vao);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();

    ImGui::NewFrame();

    ImGui::Begin("Debug");

    ImGui::SliderInt("Fps", &fps, 0, 120);
    ImGui::SliderFloat3("Camera Position", &cameraPosition[0], -10, 10);

    if ((time - prevRenderTime) >= renderRate)
    {
      projection = glm::perspective(glm::radians(45.f), sScreenSize.x / sScreenSize.y, 0.001f, 1000.f);
      view = glm::lookAt(cameraPosition, { 0.f, 0.f, 0.f }, { 0.f, 1.f, 0.f });
      model = glm::identity<fm4>();

      glUniformMatrix4fv(uProjection, 1, 0, &projection[0][0]);
      glUniformMatrix4fv(uView, 1, 0, &view[0][0]);
      glUniformMatrix4fv(uModel, 1, 0, &model[0][0]);

      glDrawElements(GL_TRIANGLES, quadLayout.IndexCount, GL_UNSIGNED_INT, 0);

      prevRenderTime = time;
    }

    ImGui::End();

    ImGui::EndFrame();

    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(pWindow);

    prevTime = time;
  }

  glfwTerminate();

  return sStatus;
}