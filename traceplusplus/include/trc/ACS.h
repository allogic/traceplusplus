#pragma once

#include "Core.h"

class ACS
{
public:
  struct TEvent;
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

  };
  struct TComponent
  {
    virtual void Update(const r32 deltaTime) {}
    virtual void Render(const r32 deltaTime) const {}
    virtual void Event(const TEvent& event) {}
  };
  struct TActor
  {

  };

  static void Debug();

  template<typename ... Args>
  inline static TActor* Create(Args&&... arg)
  {
    auto pActor = new TActor(std::forward<Args>(arg)...);
    auto [_, rc] = sActors.emplace(TMutableKey{ pActor }, TComponents{});
    return rc ? pActor : nullptr;
  }

  template<typename T, typename ... Args>
  //requires std::is_base_of_v<TComponent, T>
  inline static TComponent* Attach(TActor* pActor, Args&&... arg)
  {
    auto it = sActors.find(pActor);

    if (it != sActors.end())
    {
      it->first.id |= ToMaskBit<T>();
      auto [it, rc] = it->second.emplace(typeid(T).hash_code(), new T(std::forward<Args>(arg)...));
      return rc ? it->second : nullptr;
    }

    return nullptr;
  }

  template<typename T>
  //requires std::is_base_of_v<TComponent, T>
  inline static TComponent* Obtain(TActor* pActor)
  {
    auto it = sActors.find(pActor);

    if (it != sActors.end())
    {
      auto& components = it->second;
      auto it = components.find(typeid(T).hash_code());
      if (it != components.end()) return it->second;
    }

    return nullptr;
  }

private:
  template<typename T>
  //requires std::is_base_of_v<TComponent, T>
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