#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "trc/Trc.h"

#include <string>
#include <vector>

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
  enum class TButton      : s32 { None = -1, Left, Right, Middle };
  enum class TButtonState : s32 { None = -1, Release, Press, Hold };

  TMouse::TButton      button           = TMouse::TButton::None;
  TMouse::TButtonState state            = TMouse::TButtonState::None;
  u32                  isMoving         = 0;
  b8                   invertX          = 0;
  b8                   invertY          = 0;
  r32v2                position         = {};
  r32v2                deltaPosition    = {};
  r32v2                positionRegister = {};
  r32v2                positionPrevious = {};

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

static s32     sStatus = 1;
static TScreen sScreen = { { 1280.f, 720.f } };
static TMouse  sMouse  = { 1, 0, sScreen.screenSize / 2.f };

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
    sScreen.aspectRatio = (r32)width / height;
    glViewport(0, 0, width, height);
    });
  glfwSetCursorPosCallback(pWindow, [](GLFWwindow* pWin, r64 x, r64 y) {
    sMouse.positionPrevious = sMouse.position;
    sMouse.position = { (r32)x, (r32)y };
    sMouse.isMoving = 1;
    });
  glfwSetMouseButtonCallback(pWindow, [](GLFWwindow* pWin, s32 button, s32 action, s32 mods) {
    sMouse.button = (TMouse::TButton)button;
    sMouse.state = (TMouse::TButtonState)action;

    //if (sMouse.button == TMouse::TButton::Right)
    //{
    //  switch (sMouse.state)
    //  {
    //  case TMouse::TButtonState::Press:
    //    //glfwSetInputMode(pWin, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    //    sMouse.positionRegister = sMouse.position;
    //    //sMouse.position = sScreen.screenSize / 2.f;
    //    //sMouse.positionPrevious = sMouse.position;
    //    break;
    //  case TMouse::TButtonState::Hold:
    //    sMouse.deltaPosition = sMouse.position - sMouse.positionPrevious;
    //    pWorld->camera.rotationDrag.x = sMouse.invertX ? -sMouse.deltaPosition.x : sMouse.deltaPosition.x; // aspect
    //    pWorld->camera.rotationDrag.y = sMouse.invertY ? -sMouse.deltaPosition.y : sMouse.deltaPosition.y;
    //    break;
    //  case TMouse::TButtonState::Release:
    //    sMouse.position = sMouse.positionRegister;
    //    //sMouse.positionPrevious = sMouse.position;
    //    pWorld->camera.rotationDrag = { 0.f, 0.f };
    //    //glfwSetInputMode(pWin, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    //    break;
    //  }
    //}
    });

  TVertexBufferLayout quadLayout(4, 5, 6);
  quadLayout.pVertices[0] = -.5f;
  quadLayout.pVertices[1] = -.5f;
  quadLayout.pVertices[2] = 0.f;
  //quadLayout.pVertices[3] = 0.f;
  //quadLayout.pVertices[4] = 0.f;

  quadLayout.pVertices[5] = .5f;
  quadLayout.pVertices[6] = -.5f;
  quadLayout.pVertices[7] = 0.f;
  //quadLayout.pVertices[8] = 1.f;
  //quadLayout.pVertices[9] = 0.f;

  quadLayout.pVertices[10] = -.5f;
  quadLayout.pVertices[11] = .5f;
  quadLayout.pVertices[12] = 0.f;
  //quadLayout.pVertices[13] = 0.f;
  //quadLayout.pVertices[14] = 1.f;

  quadLayout.pVertices[15] = .5f;
  quadLayout.pVertices[16] = .5f;
  quadLayout.pVertices[17] = 0.f;
  //quadLayout.pVertices[18] = 1.f;
  //quadLayout.pVertices[19] = 1.f;

  quadLayout.pIndices[0] = 3;
  quadLayout.pIndices[1] = 1;
  quadLayout.pIndices[2] = 2;
  quadLayout.pIndices[3] = 3;
  quadLayout.pIndices[4] = 2;
  quadLayout.pIndices[5] = 0;

  //if (!CreateBuffers(quadLayout))
  //  return FAILED_CREATING_BUFFER;
  //
  //u32 shader = 0;
  //
  //if (!CompileShaders(shader, TraceVertexSource, TraceFragmentSource))
  //  return FAILED_LOADING_SHADER;

  //u32 uProjection;
  //u32 uView;
  //u32 uModel;

  //if (uProjection = glGetUniformLocation(shader, "uProjection"); uProjection < 0)
  //  return FAILED_SHADER_UNIFORM;
  //
  //if (uView = glGetUniformLocation(shader, "uView"); uView < 0)
  //  return FAILED_SHADER_UNIFORM;
  //
  //if (uModel = glGetUniformLocation(shader, "uModel"); uModel < 0)
  //  return FAILED_SHADER_UNIFORM;

  for (u32 i = 0; i < 1024; i++)
  {
    auto pPortalActor = TACS::Create();
    TACS::Attach<TTransform>(pPortalActor);
    TACS::Attach<TMesh>(pPortalActor, &quadLayout);
  }

  auto pCameraActor = TACS::Create();
  TACS::Attach<TTransform>(pCameraActor);
  TACS::Attach<TCamera>(pCameraActor);

  struct TCameraController : TACS::TComponent
  {
    TTransform* pTransform = nullptr;
    TCamera*    pCamera    = nullptr;

    TCameraController()
    {
      pTransform = (TTransform*)TACS::Obtain<TTransform>(this);
      pCamera = (TCamera*)TACS::Obtain<TCamera>(this);
    }

    void Update(const r32 deltaTime) override
    {
      if (glm::length(pCamera->rotationVelocity) > pCamera->rotationDeadzone)
        pCamera->rotationVelocity += -pCamera->rotationVelocity * pCamera->rotationDecay * deltaTime;
      else
        pCamera->rotationVelocity = { 0.f, 0.f };

      pCamera->rotationVelocity += pCamera->rotationDrag * pCamera->rotationSpeed * deltaTime;

      if (pCamera->rotationVelocity.x > 180.f) pCamera->rotationVelocity.x = -180.f;
      if (pCamera->rotationVelocity.x < -180.f) pCamera->rotationVelocity.x = 180.f;

      if (pCamera->rotationVelocity.y > 180.f) pCamera->rotationVelocity.y = -180.f;
      if (pCamera->rotationVelocity.y < -180.f) pCamera->rotationVelocity.y = 180.f;

      r32m4 localRotation = glm::identity<r32m4>();
      localRotation = glm::rotate(localRotation, glm::radians(pCamera->rotationVelocity.y), pCamera->localRight);
      localRotation = glm::rotate(localRotation, glm::radians(pCamera->rotationVelocity.x), pCamera->localUp);

      pCamera->localRight = localRotation * r32v4{ pCamera->localRight, 1.f };
      pCamera->localUp = localRotation * r32v4{ pCamera->localUp, 1.f };
      pCamera->localForward = localRotation * r32v4{ pCamera->localForward, 1.f };
    }
  };

  TACS::Attach<TCameraController>(pCameraActor);

  r32 time = 0.f;
  r32 prevTime = 0.f;
  r32 deltaTime = 0.f;

  s32 fps = 80;
  r32 renderRate = 1.f / fps;
  r32 prevRenderTime = 0.f;

  r32m4 projection = glm::identity<r32m4>();
  r32m4 view = glm::identity<r32m4>();
  r32m4 model = glm::identity<r32m4>();

  while (sStatus > 0)
  {
    time = (r32)glfwGetTime();
    deltaTime = time - prevTime;
    renderRate = 1.f / fps;

    //if (glfwGetKey(pWindow, GLFW_KEY_W) == GLFW_PRESS) world.camera.transform.position += world.camera.localForward * world.camera.positionSpeed * deltaTime;
    //if (glfwGetKey(pWindow, GLFW_KEY_S) == GLFW_PRESS) world.camera.transform.position -= world.camera.localForward * world.camera.positionSpeed * deltaTime;
    //if (glfwGetKey(pWindow, GLFW_KEY_A) == GLFW_PRESS) world.camera.transform.position += world.camera.localRight * world.camera.positionSpeed * deltaTime;
    //if (glfwGetKey(pWindow, GLFW_KEY_D) == GLFW_PRESS) world.camera.transform.position -= world.camera.localRight * world.camera.positionSpeed * deltaTime;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();

    ImGui::NewFrame();

    ImGui::Begin("Debug");
    ImGui::SliderInt("Fps", &fps, 0, 120);
    ImGui::End();

    //ImGui::Begin("Mouse");
    //ImGui::LabelText("Button", "%d", sMouse.button);
    //ImGui::LabelText("State", "%d", sMouse.state);
    //ImGui::LabelText("Is Moving", "%d", sMouse.isMoving);
    //ImGui::Checkbox("Invert X", &sMouse.invertX);
    //ImGui::Checkbox("Invert Y", &sMouse.invertY);
    //ImGui::LabelText("Position", "{%3.3f, %3.3f}", sMouse.position.x, sMouse.position.y);
    //ImGui::LabelText("Position Previous", "{%3.3f, %3.3f}", sMouse.positionPrevious.x, sMouse.positionPrevious.y);
    //ImGui::End();

    //ImGui::Begin("Camera");
    //ImGui::Checkbox("Projection", (b8*)(&world.camera.projection));
    //ImGui::SliderFloat3("Position", &world.camera.transform.position[0], -100.f, 100.f);
    //ImGui::SliderFloat3("Rotation", &world.camera.transform.rotation[0], -180.f, 180.f);
    //ImGui::SliderFloat3("Scale", &world.camera.transform.scale[0], -100.f, 100.f);
    //ImGui::LabelText("Right", "{%3.3f, %3.3f, %3.3f}", world.camera.right.x, world.camera.right.y, world.camera.right.z);
    //ImGui::LabelText("Up", "{%3.3f, %3.3f, %3.3f}", world.camera.up.x, world.camera.up.y, world.camera.up.z);
    //ImGui::LabelText("Forward", "{%3.3f, %3.3f, %3.3f}", world.camera.forward.x, world.camera.forward.y, world.camera.forward.z);
    //ImGui::LabelText("Local Right", "{%3.3f, %3.3f, %3.3f}", world.camera.localRight.x, world.camera.localRight.y, world.camera.localRight.z);
    //ImGui::LabelText("Local Up", "{%3.3f, %3.3f, %3.3f}", world.camera.localUp.x, world.camera.localUp.y, world.camera.localUp.z);
    //ImGui::LabelText("Local Forward", "{%3.3f, %3.3f, %3.3f}", world.camera.localForward.x, world.camera.localForward.y, world.camera.localForward.z);
    //ImGui::SliderFloat("Position Speed", &world.camera.positionSpeed, 0.f, 10.f);
    //ImGui::SliderFloat("Rotation Speed", &world.camera.rotationSpeed, 0.f, 10.f);
    //ImGui::LabelText("Rotation Drag", "{%3.3f, %3.3f}", world.camera.rotationDrag.x, world.camera.rotationDrag.y);
    //ImGui::SliderFloat("Rotation Decay", &world.camera.rotationDecay, 0.f, 10.f);
    //ImGui::SliderFloat("Rotation Deadzone", &world.camera.rotationDeadzone, 0.f, 1.f);
    //ImGui::SliderFloat2("Rotation Velocity", &world.camera.rotationVelocity[0], -100.f, 100.f);
    //ImGui::End();

    //ImGui::Begin("Model");
    //ImGui::SliderFloat3("Position", &quadLayout.transform.position[0], -10, 10);
    //ImGui::SliderFloat3("Rotation", &quadLayout.transform.rotation[0], -10, 10);
    //ImGui::SliderFloat3("Scale", &quadLayout.transform.scale[0], -10, 10);
    //ImGui::End();

    TACS::Debug();
    TACS::Update(deltaTime);

    if ((time - prevRenderTime) >= renderRate)
    {
      glClearColor(0.f, 0.f, 0.f, 1.f);
      glClear(GL_COLOR_BUFFER_BIT);

      TACS::Render(deltaTime);
      //glUseProgram(shader);
      //glBindVertexArray(quadLayout.vao);

      //projection = world.camera.Projection(sScreen.aspectRatio);
      //view = world.camera.View();
      //model = glm::identity<r32m4>();

      //quadLayout.transform.rotation.y += 2.f * deltaTime;

      //model = glm::translate(model, quadLayout.transform.position);
      //model = glm::rotate(model, quadLayout.transform.rotation.x, { 1.f, 0.f, 0.f });
      //model = glm::rotate(model, quadLayout.transform.rotation.y, { 0.f, 1.f, 0.f });
      //model = glm::rotate(model, quadLayout.transform.rotation.z, { 0.f, 0.f, 1.f });
      //model = glm::scale(model, quadLayout.transform.scale);

      //glUniformMatrix4fv(uProjection, 1, 0, &projection[0][0]);
      //glUniformMatrix4fv(uView, 1, 0, &view[0][0]);
      //glUniformMatrix4fv(uModel, 1, 0, &model[0][0]);

      //glDrawElements(GL_TRIANGLES, quadLayout.indexCount, GL_UNSIGNED_INT, 0);

      prevRenderTime = time;
    }

    sMouse.isMoving = 0;

    ImGui::EndFrame();
    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(pWindow);
    glfwPollEvents();

    prevTime = time;
  }

  glfwTerminate();

  return sStatus;
}