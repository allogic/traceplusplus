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

struct TVertexArrayLayout
{

};

#include "Tracer.h"

#include <string>
#include <fstream>
#include <sstream>

#define FAILED_GLFW_INIT       -1
#define FAILED_GLFW_WINDOW     -2
#define FAILED_GL_CONTEXT      -3
#define FAILED_IMGUI_VERSION   -4
#define FAILED_IMGUI_CONTEXT   -5
#define FAILED_IMGUI_IMPL_GLFW -6
#define FAILED_IMGUI_IMPL_GL   -7
#define FAILED_LOADING_SHADER  -8
#define FAILED_SHADER_UNIFORM  -9

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
//    return true;
//  }
//
//  b8 OnUserUpdate(f32 elapsedTime) override
//  {
//    Clear({ 0, 0, 0, 255 });
//
//    const glm::fvec2 screenSize = { ScreenWidth(), ScreenHeight() };
//
//    glm::fvec2 screenUvNorm;
//    glm::fvec3 rayOrig;
//    glm::fvec3 hitNormal;
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
//    return true;
//  }
//
//private:
//  glm::fvec3 mCameraRotation{ 0, 0, 0 };
//
//  CCamera mCamera{ { 0, 0, -1 }, { 0, 0, 1 }, { 0, 1, 0 } };
//
//  CScene mScene;
//
//  CCube mCube{ { 0, 0, 5 }, { 5, 5, 5 } };
//  CSphere mSphere{ { 0, 0, 0 }, 0.5f };
//};

constexpr const s8 TraceVertexSource[] = R"glsl(
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
)glsl";

constexpr const s8 TraceFragmentSource[] = R"glsl(
in vec2 iScreenSize;

out vec4 oFragColor;

void main()
{
  oFragColor = vec4(iScreenSize, 0., 1.);
}
)glsl";

s32 CheckStatus(s32 shader, s32 type, std::string& log)
{
  s32 compileInfo;
  s32 compileInfoSize;

  glGetShaderiv(shader, type, &compileInfo);
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &compileInfoSize);

  if (compileInfoSize > 0)
  {
    log.resize(compileInfoSize);

    glGetShaderInfoLog(shader, compileInfoSize, nullptr, &log[0]);

    return false;
  }

  return true;
}

s32 LinkShaders(u32 program, u32 vertexShader, u32 fragmentShader)
{
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);

  glLinkProgram(program);

  std::string log;

  if (!CheckStatus(program, GL_LINK_STATUS, log))
  {
    std::printf(log.c_str());

    return false;
  }

  return true;
}

s32 CompileShader(u32 shader, const std::string& source)
{
  std::string finalSource = std::string{ "#version 460\n" + source };
  const char* pSource = finalSource.c_str();

  glShaderSource(shader, 1, &pSource, nullptr);
  glCompileShader(shader);

  std::string log;

  if (!CheckStatus(shader, GL_COMPILE_STATUS, log))
  {
    std::printf(log.c_str());

    return false;
  }

  return true;
}

s32 CompileShaders(u32& program, const std::string& vertexSource, const std::string& fragmentSource)
{
  s32 vertexShader = glCreateShader(GL_VERTEX_SHADER);

  if (!CompileShader(vertexShader, vertexSource))
    return false;

  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

  if (!CompileShader(fragmentShader, fragmentSource))
    return false;

  program = glCreateProgram();

  if (!LinkShaders(program, vertexShader, fragmentShader))
    return false;

  return true;
}

static s32        sStatus = 1;
static glm::fvec2 sScreenSize = { 1280, 720 };

s32 main()
{
  GLFWwindow* pWindow = nullptr;

  if (!glfwInit())
    return FAILED_GLFW_INIT;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_SAMPLES, 0);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, 1);

  if (pWindow = glfwCreateWindow((s32)sScreenSize.x, (s32)sScreenSize.y, "Tracer", nullptr, nullptr), !pWindow)
    return FAILED_GLFW_WINDOW;

  glfwMakeContextCurrent(pWindow);

  glfwSetWindowCloseCallback(pWindow, [](GLFWwindow*) { sStatus = 0; });
  glfwSetWindowSizeCallback(pWindow, [](GLFWwindow*, s32 width, s32 height) { sScreenSize = { width, height }; });

  glfwSwapInterval(0);

  if (!gladLoadGL())
    return FAILED_GL_CONTEXT;

  if (!IMGUI_CHECKVERSION())
    return FAILED_IMGUI_VERSION;

  if (!ImGui::CreateContext())
    return FAILED_IMGUI_CONTEXT;

  volatile ImGuiIO& io = ImGui::GetIO();
  volatile ImGuiStyle& style = ImGui::GetStyle();
  style.WindowRounding = 0.f;
  style.WindowBorderSize = 0.f;

  if (!ImGui_ImplGlfw_InitForOpenGL(pWindow, true))
    return FAILED_IMGUI_IMPL_GLFW;

  if (!ImGui_ImplOpenGL3_Init("#version 460 core"))
    return FAILED_IMGUI_IMPL_GL;

  u32 program;

  if (!CompileShaders(program, TraceVertexSource, TraceFragmentSource))
    return FAILED_LOADING_SHADER;

  u32 uProjection;
  u32 uView;

  if (uProjection = glGetUniformLocation(program, "uProjection"); !uProjection)
    return FAILED_SHADER_UNIFORM;

  if (uView = glGetUniformLocation(program, "uView"); !uView)
    return FAILED_SHADER_UNIFORM;

  f32 time = 0.f;
  f32 prevTime = 0.f;
  f32 deltaTime = 0.f;

  s32 fps = 60;
  f32 renderRate = 1.f / fps;
  f32 prevRenderTime = 0.f;

  glm::fvec3 cameraPosition = { 0.f, 0.f, 0.f };

  glm::fmat4 projection;
  glm::fmat4 view;

  while (sStatus > 0)
  {
    glfwPollEvents();

    renderRate = 1.f / fps;

    time = (f32)glfwGetTime();
    deltaTime = time - prevTime;

    glClearColor(1.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();

    ImGui::NewFrame();

    ImGui::Begin("Debug");

    ImGui::SliderInt("Fps", &fps, 0, 120, "%d");

    if ((time - prevRenderTime) >= renderRate)
    {
      projection = glm::perspective(glm::radians(45.f), sScreenSize.x / sScreenSize.y, 0.0001f, 1000.f);
      view = glm::lookAt({ 0.f, 0.f, 1.f }, cameraPosition, { 0.f, 1.f, 0.f });

      glUniformMatrix4fv(uProjection, 1, false, &projection[0][0]);
      glUniformMatrix4fv(uView, 1, false, &view[0][0]);

      prevRenderTime = time;
    }

    ImGui::End();

    ImGui::EndFrame();

    glViewport(0, 0, (s32)sScreenSize.x, (s32)sScreenSize.y);

    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(pWindow);

    prevTime = time;
  }

  glfwTerminate();

  return sStatus;
}