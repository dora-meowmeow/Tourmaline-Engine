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
#include <cstdlib>
#include <type_traits>
#include <typeindex>
#include <typeinfo>
#include <utility>

#include "../Concepts.hpp"
#include "../Containers/DualkeyMap.hpp"
#include "../Containers/Hashmap.hpp"
#include "../Types.hpp"
#include "Corrade/Containers/Array.h"
#include "Corrade/Containers/Containers.h"
#include "Corrade/Containers/Function.h"
#include "Corrade/Tags.h"
#include "ECS/BuiltinComponents.hpp"
#include "Logging.hpp"
#include "Random.hpp"

namespace Tourmaline::Systems::ECS {
using Entity = Tourmaline::Type::UUID;
using System = Tourmaline::Type::UUID;

class World {
public:
  World() {}
  // ====== World controls ======
  void Step();

  // ========  Entities  ========
  [[nodiscard]]
  Entity CreateEntity();
  [[nodiscard("Pointless call of EntityExists")]]
  bool EntityExists(const Entity &entity) noexcept;
  bool DestroyEntity(Entity entity);

  // ======== Systems ========
  template <typename SystemFunction>
  [[nodiscard]]
  System AddSystem(SystemFunction &&system, bool enabled = true) {
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

    // Type erasure nightmare
    System newSystem = Random::GenerateUUID();
    systemFunction internalFunction = [system](const Entity &entity,
                                               std::span<std::any *> args) {
      [&]<std::size_t... index>(std::index_sequence<index...>) {
        system(entity, (any_cast<typename Traits::template argument<index + 1>>(
                           *args[index]))...);
      }(std::make_integer_sequence<std::size_t, componentCount>{});
    };

    // Making sure arguments are valid and preparing to cache
    Corrade::Containers::Array<std::type_index> componentList{
        Corrade::DirectInit, componentCount, typeid(ECS::Component)};
    [&]<std::size_t... index>(std::index_sequence<index...>) {
      // Making sure that everything is infact a component
      static_assert(
          (!std::is_base_of_v<typename Traits::template argument<index + 1>,
                              ECS::Component> &&
           ...),
          "Every argument aside from first argument must be derived from "
          "ECS::Component");
      ((componentList[index] =
            typeid(typename Traits::template argument<index + 1>)),
       ...);
    }(std::make_integer_sequence<std::size_t, componentCount>{});

    // Registering the system
    registeredSystems.Insert(
        newSystem,
        {std::move(internalFunction), typeid(arguments),
         entityComponentMap.QueryWithAll(componentList, true), enabled});
    systemList.push_back(newSystem);
    return newSystem;
  }

  [[nodiscard("Pointless call of ListAllSystems")]]
  std::span<System> ListAllSystems();
  [[nodiscard("Pointless call of GetSystemEnable")]]
  bool GetSystemEnable(const System &system) noexcept;
  void SetSystemEnable(const System &system, bool beEnabled = true);
  bool DestroySystem(const System &system);

  // ======== Components ========
  template <isAComponent Component, typename... ComponentArgs>
  Component &AddComponent(const Entity &entity, ComponentArgs &&...args) {
    auto newComponent = entityComponentMap.Insert(entity, typeid(Component),
                                                  Component(args...));

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
  using systemComponentCache = std::vector<componentCache>;
  struct systemStorage {
    systemFunction function;
    std::type_index functionSignature;
    systemComponentCache cache;
    bool isEnabled = true;
  };

  std::vector<System> systemList;
  Containers::Hashmap<System, systemStorage> registeredSystems{};

  // ======== Life-cycle ========
  void preSystems();
  void postSystems();
};
} // namespace Tourmaline::Systems::ECS
#endif
