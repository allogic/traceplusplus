#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "trc/Trc.h"

struct TScreen
{
  r32v2 screenSize  = {};
  r32   aspectRatio = 0.f;

  TScreen(const r32v2& screenSize)
    : screenSize(screenSize)
    , aspectRatio(screenSize.x / screenSize.y) {};
  virtual ~TScreen() = default;
};
struct TMouse
{
  enum class TButtonState : s32 { None = -1, Release, Press };

  TButtonState state            = TButtonState::None;
  u32          button           = 0;
  u32          isMoving         = 0;
  b8           invertX          = 0;
  b8           invertY          = 0;
  r32v2        position         = {};
  r32v2        deltaPosition    = {};
  r32v2        positionPrevious = {};

  TMouse(b8 invertX, b8 invertY, const r32v2& position)
    : invertX(invertX)
    , invertY(invertY)
    , position(position) {}
  virtual ~TMouse() = default;
};

#define GL_MAJOR 4
#define GL_MINOR 6

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
//  b8 OnUserUpdate(r32 elapsedTime) override
//  {
//    Clear({ 0, 0, 0, 255 });
//
//    const r32v2 screenSize = { ScreenWidth(), ScreenHeight() };
//
//    r32v2 screenUvNorm;
//    r32v3 rayOrig;
//    r32v3 hitNormal;
//
//    olc::Pixel color;
//
//    mCamera.Update();
//
//    for (r32 x = 0.f; x < screenSize.x; x++)
//      for (r32 y = 0.f; y < screenSize.y; y++)
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
//  r32v3 mCameraRotation{ 0, 0, 0 };
//
//  CCamera mCamera{ { 0, 0, -1 }, { 0, 0, 1 }, { 0, 1, 0 } };
//
//  CScene mScene;
//
//  CCube mCube{ { 0, 0, 5 }, { 5, 5, 5 } };
//  CSphere mSphere{ { 0, 0, 0 }, 0.5f };
//};

static s32       sStatus   = 1;
static TScreen   sScreen   = { { 1280.f, 720.f } };
static TMouse    sMouse    = { 1, 0, sScreen.screenSize / 2.f };

s32 main()
{
  if (!glfwInit())
    return FAILED_GLFW_INIT;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GL_MAJOR);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GL_MINOR);
  glfwWindowHint(GLFW_SAMPLES, 0);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, 1);

  GLFWwindow* pWindow = nullptr;

  if (pWindow = glfwCreateWindow((s32)sScreen.screenSize.x, (s32)sScreen.screenSize.y, "Tracer", nullptr, nullptr), !pWindow)
    return FAILED_GLFW_WINDOW;

  glfwMakeContextCurrent(pWindow);

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

  if (!ImGui_ImplOpenGL3_Init("#version 460 core"))
    return FAILED_IMGUI_IMPL_GL;

  glfwSetWindowCloseCallback(pWindow, [](GLFWwindow* pWin) {
    sStatus = 0;
    });
  glfwSetWindowSizeCallback(pWindow, [](GLFWwindow* pWin, s32 width, s32 height) {
    sScreen.screenSize = { width, height };
    sScreen.aspectRatio = (width > 0 && height > 0) ? (r32)width / height : 1.f;
    glViewport(0, 0, width, height);
    });
  glfwSetCursorPosCallback(pWindow, [](GLFWwindow* pWin, r64 x, r64 y) {
    sMouse.positionPrevious = sMouse.position;
    sMouse.position = { (r32)x, (r32)y };
    sMouse.deltaPosition = sMouse.position - sMouse.positionPrevious;
    sMouse.isMoving = 1;
    });
  glfwSetMouseButtonCallback(pWindow, [](GLFWwindow* pWin, s32 button, s32 action, s32 mods) {
    ACS::Event(&TMouseButtonEvent{ action, button });
    });
  glfwSetKeyCallback(pWindow, [](GLFWwindow* pWin, s32 key, s32 scanCode, s32 action, s32 mods) {
    ACS::Event(&TKeyEvent{ action, key });
    });

  u32 triNo = 150000;
  u32 attrNo = 5;
  u32 vertexNo = triNo * 3;
  u32 indexNo = triNo * 3;

  TVertexLayout vertexLayout(vertexNo * attrNo, indexNo);

  for (u32 c = 0, v = 0, i = 0; v < vertexNo; c++, v += attrNo)
  {
    vertexLayout.pVertices[v + 0] = (std::rand() % 200) - 100.f;
    vertexLayout.pVertices[v + 1] = (std::rand() % 200) - 100.f;
    vertexLayout.pVertices[v + 2] = (std::rand() % 200) - 100.f;

    vertexLayout.pVertices[v + 3] = 0.f;
    vertexLayout.pVertices[v + 4] = 0.f;

    if (c > 0 && c % 3 == 0)
    {
      vertexLayout.pIndices[i + 0] = c - 3 + 0;
      vertexLayout.pIndices[i + 1] = c - 3 + 1;
      vertexLayout.pIndices[i + 2] = c - 3 + 2;

      i += 3;
    }
  }

  if (!vertexLayout.CreateBuffers())
    return FAILED_CREATING_BUFFER;

  TShaderLayout shaderLayout;

  if (!shaderLayout.CompileShaders(VertexDefault, FragmentDefault))
    return FAILED_LOADING_SHADER;

  auto pCameraActor = ACS::Create("Camera");
  ACS::Attach<TTransform>(pCameraActor);
  auto pCamera = ACS::Attach<TCamera>(pCameraActor, TCamera::TProjection::Perspective);
  ACS::Attach<TCameraController>(pCameraActor, pCameraActor);

  for (r32 i = 0; i < 16; i++)
    for (r32 j = 0; j < 50; j++)
    {
      r32 deg = i * (360 / 16);
      r32 ang = (deg * glm::pi<r32>() / 180);

      r32 x = glm::sin(ang) * 1000.f;
      r32 y = glm::cos(ang) * 1000.f;

      auto pCubeBufferActor = ACS::Create("Cube Buffer");
      auto pTransform = ACS::Attach<TTransform>(pCubeBufferActor);
      pTransform->position = { x, y, j * 1000.f + 2000.f };
      ACS::Attach<TMesh>(pCubeBufferActor, &vertexLayout);
      ACS::Attach<TShader>(pCubeBufferActor, &shaderLayout); // TODO: Only attachment 'TLambertShader'
      ACS::Attach<TRenderController>(pCubeBufferActor, pCubeBufferActor);
    }

  u32 targetFps = 60;
  u32 frameCount = 0;
  u32 currFps = 0;

  r32 time = 0.f;
  r32 prevTime = 0.f;
  r32 deltaTime = 0.f;

  r32 prevMeasureTime = 0.f;

  r32 renderRate = 1.f / targetFps;

  while (sStatus > 0)
  {
    frameCount++;
    
    prevTime = time;
    time = (r32)glfwGetTime();
    deltaTime = time - prevTime;

    // TODO: support framebuffer objects

    if ((time - prevMeasureTime) >= 1.f)
    {
      prevMeasureTime = time;
      currFps = glm::min(targetFps, frameCount);
      frameCount = 0;
    }

    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();

    ImGui::NewFrame();

    ImGui::Begin("Debug");
    ImGui::SliderInt("Target Fps", (s32*)&targetFps, 0, 120);
    ImGui::LabelText("Current Fps", "%d", currFps);
    ImGui::End();

    ImGui::Begin("Mouse");
    ImGui::LabelText("State", "%d", sMouse.state);
    ImGui::LabelText("Button", "%d", sMouse.button);
    ImGui::LabelText("Is Moving", "%d", sMouse.isMoving);
    ImGui::Checkbox("Invert X", &sMouse.invertX);
    ImGui::Checkbox("Invert Y", &sMouse.invertY);
    ImGui::LabelText("Position", "{%3.3f, %3.3f}", sMouse.position.x, sMouse.position.y);
    ImGui::LabelText("Position Previous", "{%3.3f, %3.3f}", sMouse.positionPrevious.x, sMouse.positionPrevious.y);
    ImGui::LabelText("Delta Position", "{%3.3f, %3.3f}", sMouse.deltaPosition.x, sMouse.deltaPosition.y);
    ImGui::End();

    Renderer::Debug();
    ACS::Debug();

    ACS::Update(deltaTime);

    Renderer::Flush();
    ACS::Render();
    Renderer::Render((TCamera*)pCamera);

    sMouse.isMoving = 0;

    ImGui::EndFrame();
    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(pWindow);
    glfwPollEvents();
  }

  glfwTerminate();

  return sStatus;
}