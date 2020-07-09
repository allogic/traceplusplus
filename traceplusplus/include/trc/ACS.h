#pragma once

#include "Core.h"

struct TACS
{
  struct TComponent;

  typedef std::map<std::size_t, TComponent*> TComponents;

  struct TEvent
  {

  };
  struct TComponent
  {
    TComponents* pComponents = nullptr;

    TComponent() = default;
    virtual ~TComponent() = default;

    virtual void Update(const r32 deltaTime) {}
    virtual void Render(const r32 deltaTime) const {}
    virtual void Event(const TEvent& event) {}
  };
  struct TActor
  {
    TComponents* pComponents = nullptr;

    TActor() = default;
    virtual ~TActor() { delete pComponents; };
  };

  inline static std::unordered_map<u64, TActor*> sMask2Actor = {};
  inline static std::map<TActor*, u64>           sActor2Mask = {};

  template<typename T>
  requires std::is_base_of_v<TComponent, T>
  inline static u64 Component2Id()
  {
    static u32 count = 0;
    static std::map<std::size_t, u64> ids = {};

    auto it = ids.find(typeid(T).hash_code());
    
    if (it == ids.end())
      return ids.emplace(typeid(T).hash_code(), 1 << count++).second;

    return it->second;
  }

  template<typename ... Args>
  inline static TActor* Create(Args&&... arg)
  {
    auto pActor = new TActor(std::forward<Args>(arg)...);
    pActor->pComponents = new TComponents;

    sActors.emplace(0, pActor);

    return pActor;
  }

  template<typename T, typename ... Args>
  requires std::is_base_of_v<TComponent, T>
  inline static TComponent* Attach(TActor* pActor, Args&&... arg)
  {
    auto it = sActor2Mask.find(pActor);

    if (it != sActor2Mask.end())
    {
      auto& mask = it->second;
      auto id = Component2Id<T>();

      if (!(mask & id))
      {
        mask |= id;
        auto [it, rc] = (*it)->pComponents->emplace(typeid(T).hash_code(), new T(std::forward<Args>(arg)...));
        return rc ? it->second : nullptr;
      }
    }

    return nullptr;
  }

  template<typename T>
  requires std::is_base_of_v<TComponent, T>
  inline static TComponent* Obtain(TActor* pActor)
  {
    auto it = sActor2Mask.find(pActor);

    if (it != sActor2Mask.end())
    {
      auto it = pActor->pComponents->find(typeid(T).hash_code());
      if (it != pActor->pComponents->end()) return it->second;
    }

    return nullptr;
  }

  static void        Debug();
  inline static void Update(const r32 deltaTime)
  {
    //for (auto pActor : sActors)
    //  for (auto [hash, pComponent] : *pActor->pComponents)
    //    pComponent->Update(deltaTime);
  }
  inline static void Render(const r32 deltaTime)
  {
    //for (auto pActor : sActors)
    //  for (auto [hash, pComponent] : *pActor->pComponents)
    //    pComponent->Render(deltaTime);
  }
};