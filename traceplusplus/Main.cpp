#include <glm/glm.hpp>

#include "Camera.h"
#include "Scene.h"
#include "Renderer.h"
#include "Math.h"

#include <iostream>
#include <string>

const float PI = 3.14159265f;

class CTracer
  : public olc::PixelGameEngine
{
public:
  CTracer() { sAppName = "Tracer"; }

public:
  bool OnUserCreate() override
  {
    //scene.AddObject(&cube);
    mScene.AddObject(&mSphere);

    return true;
  }

  bool OnUserUpdate(float elapsedTime) override
  {
    Clear({ 0, 0, 0, 255 });

    const glm::fvec2 screenSize = { ScreenWidth(), ScreenHeight() };

    glm::fvec2 screenUvNorm;
    glm::fvec3 rayOrig;
    glm::fvec3 hitNormal;

    olc::Pixel color;

    mCamera.Update();

    for (float x = 0.f; x < screenSize.x; x++)
      for (float y = 0.f; y < screenSize.y; y++)
      {
        screenUvNorm = CCamera::NDC({ x, y }, screenSize);
        rayOrig = mCamera.ProjectToWorld(screenUvNorm);

        if (mCamera.Trace(screenUvNorm, 32, mScene, hitNormal))
        {
          const auto r = (uint8_t)glm::floor(hitNormal.r * 255.f);
          const auto g = (uint8_t)glm::floor(hitNormal.g * 255.f);
          const auto b = (uint8_t)glm::floor(hitNormal.b * 255.f);

          Draw((int)x, (int)y, { r, g, b, 255 });
        }
      }

    return true;
  }

private:
  glm::fvec3 mCameraRotation{ 0, 0, 0 };

  CCamera mCamera{ { 0, 0, -1 }, { 0, 0, 1 }, { 0, 1, 0 } };

  CScene mScene;

  CCube mCube{ { 0, 0, 5 }, { 5, 5, 5 } };
  CSphere mSphere{ { 0, 0, 0 }, 0.5f };
};

int main()
{
  CTracer tracer;

  if (tracer.Construct(128, 128, 4, 4))
    tracer.Start();

  return 0;
}