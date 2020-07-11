#pragma once

#include "Core.h"

class ACS
{
public:
  struct TComponent;
  struct TActor;

private:
  typedef std::map<std::size_t, TComponent*> TComponents;

  struct TMutableKey
  {
    mutable u128 id = 0;

    TMutableKey(TActor* pActor) : id((u128)((p64)pActor) << 64) {}

    inline auto operator < (const TMutableKey& key) const
    {
      return (id >> 64).to_ullong() < (key.id >> 64).to_ullong();
    }
  };

  inline static std::map<TMutableKey, TComponents> sActors             = {};
  inline static u32                                sComponentMaskCount = 0;

public:
  struct TEvent
  {
    enum class TEventType : s32 { None = -1, Mouse, Keyboard, Window };

    TEventType eventType = TEventType::None;

    TEvent(TEventType eventType) : eventType(eventType) {}
  };
  struct TComponent
  {
    const s8* name = nullptr;

    TComponent(const s8* name) : name(name) {}

    virtual void Update(const r32 deltaTime) {}
    virtual void Render() const {}
    virtual void Event(TEvent& event) {}
    virtual void Debug() {}
  };
  struct TActor
  {
    const s8* name = nullptr;

    TActor(const s8* name) : name(name) {}
  };

  template<typename ... Args>
  inline static TActor* Create(Args&&... arg)
  {
    auto pActor = new TActor(std::forward<Args>(arg)...);
    auto [_, rc] = sActors.emplace(TMutableKey{ pActor }, TComponents{});
    return rc ? pActor : nullptr;
  }

  template<typename T, typename ... Args>
  requires std::is_base_of_v<TComponent, T>
  inline static T* Attach(TActor* pActor, Args&&... arg)
  {
    auto it = sActors.find(pActor);

    if (it != sActors.end())
    {
      it->first.id |= ToMaskBit<T>();
      auto [it, rc] = it->second.emplace(typeid(T).hash_code(), new T(std::forward<Args>(arg)...));
      return rc ? (T*)it->second : nullptr;
    }

    return nullptr;
  }

  template<typename T>
  requires std::is_base_of_v<TComponent, T>
  inline static T* Obtain(TActor* pActor)
  {
    auto it = sActors.find(pActor);

    if (it != sActors.end())
    {
      auto& components = it->second;
      auto it = components.find(typeid(T).hash_code());
      if (it != components.end()) return (T*)it->second;
    }

    return nullptr;
  }

  inline static void Update(const r32 deltaTime)
  {
    for (const auto& [key, components] : sActors)
    {
      // TODO: Traverse only if key mask matches

      for (const auto& [hash, pComponent] : components)
        pComponent->Update(deltaTime);
    }
  }
  inline static void Render()
  {
    for (const auto& [key, components] : sActors)
    {
      // TODO: Traverse only if key mask matches

      for (const auto& [hash, pComponent] : components)
        pComponent->Render();
    }
  }
  inline static void Event(TEvent* pEvent)
  {
    for (const auto& [key, components] : sActors)
    {
      // TODO: Traverse only if key mask matches

      for (const auto& [hash, pComponent] : components)
        pComponent->Event(*pEvent);
    }
  }
  static void Debug();

private:
  template<typename T>
  requires std::is_base_of_v<TComponent, T>
  inline static u64 ToMaskBit()
  {
    static std::map<std::size_t, u64> ids = {};

    auto it = ids.find(typeid(T).hash_code());

    if (it == ids.end())
    {
      auto [it, rc] = ids.emplace(typeid(T).hash_code(), 1 << sComponentMaskCount++);
      return rc ? it->second : 1;
    }

    return it->second;
  }
};