/*
 * SPDX-FileCopyrightText: Dora "cat" <cat@thenight.club>
 * SPDX-License-Identifier: MPL-2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef GUARD_TOURMALINE_ECS_H
#define GUARD_TOURMALINE_ECS_H
#include <any>
#include <cstddef>
#include <cstdlib>
#include <type_traits>
#include <typeindex>
#include <typeinfo>
#include <utility>

#include "../Concepts.hpp"
#include "../Containers/DualkeyMap.hpp"
#include "../Containers/Hashlist.hpp"
#include "../Containers/Hashmap.hpp"
#include "../Types.hpp"

#include "Corrade/Containers/Array.h"
#include "Corrade/Containers/Containers.h"
#include "Corrade/Containers/Function.h"
#include "Corrade/Containers/StaticArray.h"
#include "Corrade/Tags.h"
#include "ECS/BuiltinComponents.hpp"
#include "Logging.hpp"
#include "Random.hpp"

namespace Tourmaline::Systems::ECS {
using Entity = Tourmaline::Type::UUID;
using System = Tourmaline::Type::UUID;
enum SystemPriority { Start, Pre, Default, Post, Final };

class World {
public:
  World() {}
  // ====== World controls ======
  void Step();

  // ========  Entities  ========
  Entity CreateEntity(bool isEnabled = true);
  [[nodiscard("Pointless call of EntityExists")]]
  bool EntityExists(const Entity &entity) noexcept;
  void SetEntityEnable(const Entity &entity, bool beEnabled = true) noexcept;
  [[nodiscard("Pointless call of GetEntityEnable")]]
  bool GetEntityEnable(const Entity &entity) noexcept;
  bool DestroyEntity(Entity entity);

  // ======== Systems ========
  template <typename SystemFunction>
  System AddSystem(SystemFunction &&system, SystemPriority priority = Default,
                   bool enabled = true) {
    using Traits = Concepts::FunctionTraits<SystemFunction>;
    using returnType = Traits::returnType;
    using arguments = Traits::arguments;
    using firstArgument = Traits::template argument<0>;
    constexpr std::size_t componentCount = Traits::argumentCount - 1;

    // Welcome to defensive programming hell
    static_assert(std::is_void_v<returnType>, "Return type must be void!");
    static_assert(componentCount > 0,
                  "A System at minimum needs to have at least 1 "
                  "argument aside from const Entity&!");
    static_assert(std::is_same_v<firstArgument, const Entity &>,
                  "First Argument must be the type const Entity&!");

    // Making sure that everything is infact a component
    [&]<std::size_t... index>(std::index_sequence<index...>) {
      static_assert(
          (!std::is_base_of_v<typename Traits::template argument<index + 1>,
                              ECS::Component> &&
           ...),
          "Every argument aside from first argument must be derived from "
          "ECS::Component");
    }(std::make_integer_sequence<std::size_t, componentCount>{});

    // Type erasure nightmare
    System newSystem = Random::GenerateUUID();
    systemFunction internalFunction = [system](const Entity &entity,
                                               std::span<std::any *> args) {
      [&]<std::size_t... index>(std::index_sequence<index...>) {
        // This check could be done in World::Step(), however
        // it is easier (and cheaper I believe) to implement it here
        if ((any_cast<typename Traits::template argument<index + 1>>(
                 *args[index])
                 .isEnabled &&
             ...)) [[unlikely]] {
          system(entity,
                 (any_cast<typename Traits::template argument<index + 1>>(
                     *args[index]))...);
        }
      }(std::make_integer_sequence<std::size_t, componentCount>{});
    };

    // No need to cache something that already exists
    systemCache *newSystemCache;
    if (cacheRegistry.Has(typeid(arguments))) {
      newSystemCache = &cacheRegistry.Get(typeid(arguments));
      ++newSystemCache->userCount;
    } else {
      newSystemCache = &cacheRegistry.Insert(
          typeid(arguments),
          {typeid(arguments),
           systemArgumentArray{Corrade::DirectInit, componentCount,
                               typeid(ECS::Component)},
           {}});

      // I am sure this can be merged with first IIFE but
      // it makes it hell to work with
      auto insertToRegistries = [&](std::type_index typeId, std::size_t index) {
        newSystemCache->arguments[index] = typeId;
        if (componentCacheMap.Has(typeId)) {
          componentCacheMap.Get(typeId).emplace_back(newSystemCache);
          return;
        }
        componentCacheMap.Insert(typeId, {newSystemCache});
      };

      [&]<std::size_t... index>(std::index_sequence<index...>) {
        ((insertToRegistries(
             typeid(typename Traits::template argument<index + 1>), index)),
         ...);
      }(std::make_integer_sequence<std::size_t, componentCount>{});
    }

    // Registering the system
    systemRegistry.Insert(newSystem,
                          {std::move(internalFunction), typeid(arguments),
                           newSystemCache, priority, enabled});
    systemList[priority].push_back(newSystem);
    return newSystem;
  }

  [[nodiscard("Pointless call of ListAllSystems")]]
  std::span<std::vector<System>> ListAllSystems();
  [[nodiscard("Pointless call of GetSystemEnable")]]
  bool GetSystemEnable(const System &system) noexcept;
  void SetSystemEnable(const System &system, bool beEnabled = true);
  bool RemoveSystem(const System &system);

  // ======== Components ========
  template <isAComponent Component, typename... ComponentArgs>
  Component &AddComponent(const Entity &entity, ComponentArgs &&...args) {
    auto newComponent = entityComponentMap.Insert(entity, typeid(Component),
                                                  Component(args...));
    if (componentCacheMap.Has(typeid(Component))) {
      for (systemCache *cache : componentCacheMap.Get(typeid(Component))) {
        cache->isStoring = false;
      }
    }

    return std::any_cast<Component &>(std::get<2>(newComponent));
  }

  template <isAComponent Component>
  [[nodiscard("Pointless call of GetComponent")]]
  Component &GetComponent(const Entity &entity) {
    auto result = entityComponentMap.Query(entity, typeid(Component));
    if (result.empty()) {
      Logging::LogFormatted("Entity {} does not have component {}!",
                            "ECS/GetComponent", Logging::Error,
                            entity.asString(), typeid(Component).name());
    }
    return std::any_cast<Component &>(result.begin()->second);
  }

  template <isAComponent Component>
  [[nodiscard("Pointless call of HasComponent")]]
  bool HasComponent(const Entity &entity) {
    return entityComponentMap.Query(entity, typeid(Component)).size();
  }

  template <isAComponent Component> bool RemoveComponent(const Entity &entity) {
    return entityComponentMap.Remove(entity, typeid(Component));
  }

  // Copying is not allowed since the ECS world is meant to be
  // a session with its own private session sensitive variables
  World(const World &) = delete;
  World &operator=(const World &) = delete;

private:
  Containers::DualkeyMap<Entity, std::type_index, std::any>
      entityComponentMap{};

  // Systems
  using systemFunction = Corrade::Containers::Function<void(
      const Entity &, std::span<std::any *>)>;
  using componentCache = decltype(entityComponentMap)::MultiQueryResult<Entity>;
  using componentCacheList =
      std::vector<decltype(entityComponentMap)::MultiQueryResult<Entity>>;
  using systemArgumentArray = Corrade::Containers::Array<std::type_index>;
  using systemArgumentTupleId = std::type_index;
  using componentId = std::type_index;

  struct systemCache {
    systemArgumentTupleId Id;
    systemArgumentArray arguments;
    componentCacheList storage;
    uint32_t userCount = 1;
    bool isStoring = false;
  };

  struct systemStorage {
    systemFunction function;
    systemArgumentTupleId arguments;
    systemCache *cache;
    SystemPriority priority;
    bool isEnabled = true;
  };

  Corrade::Containers::StaticArray<SystemPriority::Final + 1,
                                   std::vector<System>>
      systemList;
  Containers::Hashmap<systemArgumentTupleId, systemCache> cacheRegistry;
  Containers::Hashmap<System, systemStorage> systemRegistry{};
  Containers::Hashmap<componentId, std::vector<systemCache *>>
      componentCacheMap;
  Containers::Hashlist<Entity> disabledEntityList;

  // ======== Life-cycle ========
  void preSystems();
  void postSystems();
};
} // namespace Tourmaline::Systems::ECS
#endif
