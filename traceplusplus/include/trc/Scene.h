#pragma once

#include "Core.h"
#include "ACS.h"

struct TScene : ACS::TInstance<TScene>
{
  using TComponents = std::map<u64, ACS::TComponent*>;

  struct TMutableKey
  {
    mutable u128 id = 0;

    TMutableKey(ACS::TActor* pActor) : id((u128)((p64)pActor) << 64) {}

    inline auto operator < (const TMutableKey& key) const
    {
      return (id >> 64).to_ullong() < (key.id >> 64).to_ullong();
    }
  };

  std::map<TMutableKey, TComponents> actors             = {};
  std::map<std::size_t, u64>         maskIds            = {};
  u32                                componentMaskCount = 0;

  template<typename ... Args>
  inline ACS::TActor* Create(Args&&... arg)
  {
    auto pActor = new ACS::TActor(std::forward<Args>(arg)...);
    auto [_, rc] = actors.emplace(TMutableKey{ pActor }, TComponents{});
    return rc ? pActor : nullptr;
  }

  template<typename T>
  requires std::is_base_of_v<ACS::TComponent, T>
  inline u64 Register()
  {
    auto it = maskIds.find(typeid(T).hash_code());

    if (it == maskIds.end())
    {
      auto [it, rc] = maskIds.emplace(typeid(T).hash_code(), 1 << componentMaskCount++);
      return rc ? it->second : 1;
    }

    return it->second;
  }

  template<typename ... T>
  inline u64 RegisterFor()
  {
    return (... | Register<T>());
  }

  template<typename T, typename TOverride = void, typename ... Args>
  requires std::is_base_of_v<ACS::TComponent, T>
  inline T* Attach(ACS::TActor* pActor, Args&&... arg)
  {
    auto it = actors.find(pActor);

    if (it != actors.end())
    {
      it->first.id |= Register<T>();

      if constexpr (std::is_same_v<TOverride, void>)
      {
        auto [it, rc] = it->second.emplace(Register<T>(), new T(std::forward<Args>(arg)...));
        return rc ? (T*)it->second : nullptr;
      }
      else
      {
        auto [it, rc] = it->second.emplace(Register<TOverride>(), new T(std::forward<Args>(arg)...));
        return rc ? (T*)it->second : nullptr;
      }
    }

    return nullptr;
  }

  template<typename T>
  requires std::is_base_of_v<ACS::TComponent, T>
  inline T* Obtain(ACS::TActor* pActor)
  {
    auto it = actors.find(pActor);

    if (it != actors.end())
    {
      auto& components = it->second;
      auto it = components.find(Register<T>());
      if (it != components.end()) return (T*)it->second;
    }

    return nullptr;
  }

  void Update(const r32 deltaTime);
  void Event(ACS::TEvent* pEvent);
  void Debug();
  void Render();
};