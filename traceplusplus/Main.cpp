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
struct TKeyboard
{
  enum class TKeyState : s32 { None = -1, Release, Press, Hold };

  TKeyState state    = TKeyState::None;
  u32       key      = 0;

  TKeyboard() = default;
  virtual ~TKeyboard() = default;
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
static TKeyboard sKeyboard = {};

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
    sMouse.deltaPosition = sMouse.position - sMouse.positionPrevious;
    sMouse.isMoving = 1;
    });
  glfwSetMouseButtonCallback(pWindow, [](GLFWwindow* pWin, s32 button, s32 action, s32 mods) {
    sMouse.state = (TMouse::TButtonState)action;
    sMouse.button = (u32)button;
    });
  glfwSetKeyCallback(pWindow, [](GLFWwindow* pWin, s32 key, s32 scanCode, s32 action, s32 mods) {
    sKeyboard.state = (TKeyboard::TKeyState)action;
    sKeyboard.key = (u32)key;
    });

  TVertexLayout quadLayout(4, 5, 6);
  quadLayout.pVertices[0] = -.5f;
  quadLayout.pVertices[1] = -.5f;
  quadLayout.pVertices[2] = 0.f;
  quadLayout.pVertices[3] = 0.f;
  quadLayout.pVertices[4] = 0.f;

  quadLayout.pVertices[5] = .5f;
  quadLayout.pVertices[6] = -.5f;
  quadLayout.pVertices[7] = 0.f;
  quadLayout.pVertices[8] = 1.f;
  quadLayout.pVertices[9] = 0.f;

  quadLayout.pVertices[10] = -.5f;
  quadLayout.pVertices[11] = .5f;
  quadLayout.pVertices[12] = 0.f;
  quadLayout.pVertices[13] = 0.f;
  quadLayout.pVertices[14] = 1.f;

  quadLayout.pVertices[15] = .5f;
  quadLayout.pVertices[16] = .5f;
  quadLayout.pVertices[17] = 0.f;
  quadLayout.pVertices[18] = 1.f;
  quadLayout.pVertices[19] = 1.f;

  quadLayout.pIndices[0] = 3;
  quadLayout.pIndices[1] = 1;
  quadLayout.pIndices[2] = 2;
  quadLayout.pIndices[3] = 3;
  quadLayout.pIndices[4] = 2;
  quadLayout.pIndices[5] = 0;

  if (!quadLayout.CreateBuffers())
    return FAILED_CREATING_BUFFER;

  TShaderLayout shaderLayout;

  if (!shaderLayout.CompileShaders(VertexDefault, FragmentDefault))
    return FAILED_LOADING_SHADER;

  struct TRenderController : TACS::TComponent
  {
    TTransform* pTransform = nullptr;
    TMesh*      pMesh      = nullptr;
    TShader*    pShader    = nullptr;
    TCamera*    pCamera    = nullptr;

    TRenderController(TACS::TActor* pActor, TACS::TActor* pCameraActor)
    {
      pMesh = (TMesh*)TACS::Obtain<TMesh>(pActor);
      pTransform = (TTransform*)TACS::Obtain<TTransform>(pActor);
      pShader = (TShader*)TACS::Obtain<TShader>(pActor);
      pCamera = (TCamera*)TACS::Obtain<TCamera>(pCameraActor);

      pTransform->position = { (std::rand() % 100) - 50, (std::rand() % 100) - 50, (std::rand() % 100) - 50 };
      pTransform->rotation = { std::rand() % 360, std::rand() % 360, std::rand() % 360 };
      pTransform->scale    = { std::rand() % 10, std::rand() % 10, 0.f };
      //pTransform->position = { .5f, .5f, 1.f };
      //pTransform->scale = { 1, 1, 0.f };
    }

    void Render(const r32 deltaTime) const override
    {
      TRenderer::Submit(TRenderer::TRenderJob{pShader->pShaderLayout->program, pMesh->pVertexLayout->vao});

      glUseProgram(pShader->pShaderLayout->program);
      glBindVertexArray(pMesh->pVertexLayout->vao);

      r32m4 model = glm::identity<r32m4>();

      model = glm::translate(model, pTransform->position);
      model = glm::rotate(model, pTransform->rotation.x, { 1.f, 0.f, 0.f });
      model = glm::rotate(model, pTransform->rotation.y, { 0.f, 1.f, 0.f });
      model = glm::rotate(model, pTransform->rotation.z, { 0.f, 0.f, 1.f });
      model = glm::scale(model, pTransform->scale);

      s32 uDeltaTime = glGetUniformLocation(pShader->pShaderLayout->program, "uDeltaTime");
      s32 uProjection = glGetUniformLocation(pShader->pShaderLayout->program, "uProjection");
      s32 uView = glGetUniformLocation(pShader->pShaderLayout->program, "uView");
      s32 uModel = glGetUniformLocation(pShader->pShaderLayout->program, "uModel");

      glUniform1f(uDeltaTime, deltaTime);
      glUniformMatrix4fv(uProjection, 1, 0, &pCamera->projectionTensor[0][0]);
      glUniformMatrix4fv(uView, 1, 0, &pCamera->viewTensor[0][0]);
      glUniformMatrix4fv(uModel, 1, 0, &model[0][0]);

      glDrawElements(GL_TRIANGLES, pMesh->pVertexLayout->indexCount, GL_UNSIGNED_INT, 0);
    }
  };

  struct TCameraController : TACS::TComponent
  {
    TTransform* pTransform = nullptr;
    TCamera* pCamera = nullptr;

    TCameraController(TACS::TActor* pActor)
    {
      pTransform = (TTransform*)TACS::Obtain<TTransform>(pActor);
      pCamera = (TCamera*)TACS::Obtain<TCamera>(pActor);
    }

    void Update(const r32 deltaTime) override
    {
      if (sMouse.isMoving && sMouse.button == GLFW_MOUSE_BUTTON_RIGHT && sMouse.state == TMouse::TButtonState::Press)
      {
        pCamera->rotationDrag.x = sMouse.invertX ? -sMouse.deltaPosition.x : sMouse.deltaPosition.x; // aspect
        pCamera->rotationDrag.y = sMouse.invertY ? -sMouse.deltaPosition.y : sMouse.deltaPosition.y;
      }
      else
      {
        pCamera->rotationDrag = { 0.f, 0.f };
      }

      if (sKeyboard.key == GLFW_KEY_W && (sKeyboard.state == TKeyboard::TKeyState::Press || sKeyboard.state == TKeyboard::TKeyState::Hold))
        pTransform->position += pCamera->localForward * pCamera->positionSpeed * deltaTime;
      if (sKeyboard.key == GLFW_KEY_S && (sKeyboard.state == TKeyboard::TKeyState::Press || sKeyboard.state == TKeyboard::TKeyState::Hold))
        pTransform->position -= pCamera->localForward * pCamera->positionSpeed * deltaTime;

      if (sKeyboard.key == GLFW_KEY_A && (sKeyboard.state == TKeyboard::TKeyState::Press || sKeyboard.state == TKeyboard::TKeyState::Hold))
        pTransform->position += pCamera->localRight * pCamera->positionSpeed * deltaTime;
      if (sKeyboard.key == GLFW_KEY_D && (sKeyboard.state == TKeyboard::TKeyState::Press || sKeyboard.state == TKeyboard::TKeyState::Hold))
        pTransform->position -= pCamera->localRight * pCamera->positionSpeed * deltaTime;

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

      pCamera->UpdateProjection(sScreen.aspectRatio);
      pCamera->UpdateView(pTransform);
    }
  };

  auto pCameraActor = TACS::Create();
  TACS::Attach<TTransform>(pCameraActor);
  TACS::Attach<TCamera>(pCameraActor, TCamera::TProjection::Perspective);
  TACS::Attach<TCameraController>(pCameraActor, pCameraActor);

  for (u32 i = 0; i < 256; i++)
  {
    auto pPortalActor = TACS::Create();
    TACS::Attach<TTransform>(pPortalActor);
    TACS::Attach<TMesh>(pPortalActor, &quadLayout);
    TACS::Attach<TShader>(pPortalActor, &shaderLayout);
    TACS::Attach<TRenderController>(pPortalActor, pPortalActor, pCameraActor);
  }

  r32 time = 0.f;
  r32 prevTime = 0.f;
  r32 deltaTime = 0.f;

  s32 fps = 120;
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

    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();

    ImGui::NewFrame();

    ImGui::Begin("Debug");
    ImGui::SliderInt("Fps", &fps, 0, 120);
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

    ImGui::Begin("Keyboard");
    ImGui::LabelText("State", "%d", sKeyboard.state);
    ImGui::LabelText("Key", "%d", sKeyboard.key);
    ImGui::End();

    TRenderer::Debug();
    TACS::Debug();

    TACS::Update(deltaTime);

    if ((time - prevRenderTime) >= renderRate)
    {
      TRenderer::Clear();
      TACS::Render(deltaTime);
      TRenderer::Render(deltaTime);

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