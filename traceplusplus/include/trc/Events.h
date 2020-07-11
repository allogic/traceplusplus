#pragma once

#include "Core.h"
#include "ACS.h"

struct TMouseButtonEvent : ACS::TEvent
{
  s32 action = 0;
  s32 button = 0;

  TMouseButtonEvent(s32 action, s32 button)
    : TEvent(TEventType::Mouse)
    , action(action)
    , button(button) {}
};

struct TKeyEvent : ACS::TEvent
{
  s32 action = 0;
  s32 key    = 0;

  TKeyEvent(s32 action, s32 key)
    : TEvent(TEventType::Keyboard)
    , action(action)
    , key(key) {}
};

struct TWindowEvent : ACS::TEvent
{
  u32 width = 0;
  u32 height = 0;

  TWindowEvent(u32 width, u32 height)
    : TEvent(TEventType::Window)
    , width(width)
    , height(height) {}
};