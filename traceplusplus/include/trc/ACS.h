#pragma once

#include "Core.h"

struct TACS
{
  struct TComponent;

  typedef std::map<std::size_t, TComponent*> TComponents;

  struct TComponent
  {
    TComponents* pComponents = nullptr;

    TComponent() = default;
    virtual ~TComponent() = default;

    virtual void Update(const r32 deltaTime) {};
    virtual void Render(const r32 deltaTime) const {};
  };
  struct TActor
  {
    TComponents* pComponents = nullptr;

    TActor() = default;
    virtual ~TActor() { delete pComponents; };
  };

  inline static std::set<TActor*> sActors = {};

  template<typename T = TActor, typename ... Args>
  requires std::is_base_of_v<TActor, T>
  inline static T* Create(Args&&... arg)
  {
    auto pActor = new T(std::forward<Args>(arg)...);
    pActor->pComponents = new TComponents;

    sActors.emplace(pActor);

    return pActor;
  }

  template<typename T, typename ... Args>
  requires std::is_base_of_v<TComponent, T>
  inline static TComponent* Attach(TACS::TActor* pActor, Args&&... arg)
  {
    auto it = sActors.find(pActor);

    if (it != sActors.end())
    {
      auto [it, rc] = (*it)->pComponents->emplace(typeid(T).hash_code(), new T(std::forward<Args>(arg)...));
      return rc ? it->second : nullptr;
    }

    return nullptr;
  }

  template<typename T>
  requires std::is_base_of_v<TComponent, T>
  inline static TComponent* Obtain(TActor* pActor)
  {
    auto it = sActors.find(pActor);

    if (it != sActors.end())
    {
      auto it = pActor->pComponents->find(typeid(T).hash_code());
      if (it != pActor->pComponents->end()) return it->second;
    }

    return nullptr;
  }

  static void        Debug();
  inline static void Update(const r32 deltaTime)
  {
    for (auto pActor : sActors)
      for (auto [hash, pComponent] : *pActor->pComponents)
        pComponent->Update(deltaTime);
  }
  inline static void Render(const r32 deltaTime)
  {
    for (auto pActor : sActors)
      for (auto [hash, pComponent] : *pActor->pComponents)
        pComponent->Render(deltaTime);
  }
};