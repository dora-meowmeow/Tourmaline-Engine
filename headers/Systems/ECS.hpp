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
#include "ECS/Prefab.hpp"

#include "Corrade/Containers/Array.h"
#include "Corrade/Containers/Containers.h"
#include "Corrade/Containers/Function.h"
#include "Corrade/Containers/StaticArray.h"
#include "Corrade/Containers/String.h"
#include "Corrade/Containers/StringView.h"
#include "Corrade/Tags.h"
#include "ECS/BuiltinComponents.hpp"
#include "Logging.hpp"
#include "Random.hpp"

/**
 * @file
 * @brief Built-in Entity Component System.
 */

namespace Tourmaline::Systems::ECS {
/// @brief Alias for Tourmaline::Type::UUID. Entity UUIDs and System UUIDs are
/// stored seperately.
using Entity = Tourmaline::Type::UUID;
/// @brief Alias for Tourmaline::Type::UUID. Entity UUIDs and System UUIDs are
/// stored seperately.
using System = Tourmaline::Type::UUID;

/// @brief The priority in which a system will run. Start will run first and
/// Final will run last. The default is Default.
enum SystemPriority { Start, Pre, Default, Post, Final };

/**
 * @brief The fundamental class that owns the entire ECS system.
 */
class World {
public:
  World() {}
  // ====== World controls ======
  /**
   * @brief Runs each system for one time. Could be interpeted as a
   * "World Tick".
   */
  void Step();

  // ========  Entities  ========
  /**
   * @brief Creates an entity with
   * Tourmaline::Systems::Components::Transform component.
   *
   * @param isEnabled Whether or not the entity should be enabled by creation.
   * @param presetUUID Useful if you need an entity to have a specific UUID.
   *
   * @return The created entity's UUID
   */
  Entity CreateEntity(bool isEnabled = true, Type::UUID presetUUID = {0, 0});

  /**
   * @brief Creates an entity with
   * Tourmaline::Systems::Components::Transform component and also
   * components inside Tourmaline::Systems::ECS::Prefab with specific
   * values set per component.
   *
   * @tparam Components This field should be automatically filled by
   * prefab itself.
   *
   * @param prefab Prefab to use for creation of the entity.
   * @param isEnabled Whether or not the entity should be enabled by creation.
   * @param presetUUID Useful if you need an entity to have a specific UUID.
   *
   * @return The created entity's UUID
   */
  template <isAComponent... Components>
  Entity CreateFromPrefab(const Prefab<Components...> &prefab,
                          bool isEnabled = true,
                          Type::UUID presetUUID = {0, 0}) {
    Entity entity = CreateEntity(isEnabled, presetUUID);

    using tupleSignature = decltype(prefab)::tupleSignature;
    [&]<std::size_t... index>(std::index_sequence<index...>) {
      // Until I add DKM each component will be added individually
      (AddComponent<std::tuple_element_t<index, tupleSignature>>(
           entity, std::get<index>(prefab.GetTuple())),
       ...);
    }(std::make_index_sequence<std::tuple_size_v<tupleSignature>>{});

    return entity;
  }

  /**
   * @brief Checks if an entity UUID is valid.
   *
   * @param entity Entity UUID to check.
   *
   * @return True if entity exists, false otherwise.
   */
  [[nodiscard("Pointless call of EntityExists")]]
  bool EntityExists(const Entity &entity) noexcept;

  /**
   * @brief Destroys an entity if exists.
   *
   * @param entity Entity UUID to attempt to destroy.
   *
   * @return True if entity did exist and got destroyed, otherwise false.
   */
  bool DestroyEntity(Entity entity);

  /**
   * @brief Sets if an entity is enabled to be used by a system.
   *
   * @param entity Entity UUID to enable/disable.
   * @param beEnabled True to enable, false to disable.
   */
  void SetEntityEnable(const Entity &entity, bool beEnabled = true) noexcept;

  /**
   * @brief Fetches if an entity is enabled or disabled.
   *
   * @param entity Entity UUID to check.
   *
   * @return True to enable, false to disable.
   */
  [[nodiscard("Pointless call of GetEntityEnable")]]
  bool GetEntityEnable(const Entity &entity) noexcept;

  /**
   * @brief Sets an entity a label/name.
   *
   * @param entity Entity UUID to set a label/name.
   * @param label The label/name to use.
   *
   */
  void SetEntityLabel(const Entity &entity, Corrade::Containers::String label);

  /**
   * @brief Fetches entity's label/name, if available.
   *
   * @param entity Entity UUID to fetch the label.
   *
   * @return If labeled, returns the labeled name. Otherwise returns "unknown".
   */
  [[nodiscard("Pointless call of GetEntityLabel")]]
  Corrade::Containers::StringView GetEntityLabel(const Entity &entity) noexcept;

  // ======== Systems ========
  /**
   * @brief Overload of AddSystem for convenience.
   *
   * This is an overload for sake of convenience when specifying
   * a class instance. It doesn't do anything different except
   * change the order of arguments.
   */
  template <typename SystemFunction, typename Instance>
  System AddSystem(SystemFunction &&system, Instance *instance,
                   SystemPriority priority = Default, bool enabled = true) {
    return AddSystem(system, priority, enabled, instance);
  }

  /**
   * @brief Adds any function that follows the rules specified as a system.
   *
   * @tparam SystemFunction A function (could be a member function of a class)
   * that follows the rules specified below.
   * @tparam Instance If the function is a member function of a class, set it as
   * the class type. Otherwise leave it as is.
   *
   * @param system The function to add as a system.
   * @param priority Refer to Tourmaline::System::ECS::SystemPriority.
   * @param enabled Should the system be enabled from creation.
   * @param instance A pointer to a class instance. Needed for memeber functions
   * of a class that AREN'T static (a.k.a. context to run the function on).
   *
   * @return System UUID of the created system.
   *
   * ## A function must follow the following rules to be a system.
   * - Must have return type of void.
   * - Must have at least 2 arguments.
   *   - First argument must be const Entity& (or const
   * Tourmaline::Systems::ECS::Entity&).
   *   - Every argument except first must be a type that publically
   * inherit Tourmaline::Systems::ECS::Component.
   * - If a pointer-to-member function (a.k.a. member function of a class),
   * there must be an instance of a class specified.
   */
  template <typename SystemFunction, typename Instance = Type::UnspecifiedType>
  System AddSystem(SystemFunction &&system, SystemPriority priority = Default,
                   bool enabled = true, Instance *instance = nullptr) {
    using Traits = Concepts::FunctionTraits<SystemFunction>;
    using returnType = Traits::returnType;
    using arguments = Traits::arguments;
    using firstArgument = Traits::template argument<0>;
    constexpr std::size_t componentCount = Traits::argumentCount - 1;
    constexpr bool requiresInstance =
        std::is_member_function_pointer_v<SystemFunction>;

    // Welcome to defensive programming hell
    static_assert(std::is_void_v<returnType>, "Return type must be void!");
    static_assert(componentCount > 0,
                  "A System at minimum needs to have at least 1 "
                  "argument aside from const Entity&!");
    static_assert(std::is_same_v<firstArgument, const Entity &>,
                  "First Argument must be the type const Entity&!");
    if constexpr (requiresInstance) {
      static_assert(std::is_class_v<Instance>,
                    "Non-static pointer-to-member functions must supply which "
                    "instance to run the function on!");
    }

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
    systemFunction internalFunction =
        [system, instance, requiresInstance](const Entity &entity,
                                             std::span<std::any *> args) {
          [&]<std::size_t... index>(std::index_sequence<index...>) {
            // This check could be done in World::Step(), however
            // it is easier (and cheaper I believe) to implement it here
            if ((any_cast<typename Traits::template argument<index + 1>>(
                     *args[index])
                     .isEnabled &&
                 ...)) [[likely]] {

              // Pointer to Member functions
              if constexpr (requiresInstance) {
                (instance->*system)(
                    entity,
                    (any_cast<typename Traits::template argument<index + 1>>(
                        *args[index]))...);
              } else {
                system(entity,
                       (any_cast<typename Traits::template argument<index + 1>>(
                           *args[index]))...);
              }
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

  /**
   * @brief Gives a list of all systems added.
   *
   * @return A view of a list of vectors. There is in total 5 entries.
   * first entry is systems that have priority highest and 5 is last.
   *
   * @note Refer to Tourmaline::System::ECS::SystemPriority for
   * more information.
   */
  [[nodiscard("Pointless call of ListAllSystems")]]
  std::span<std::vector<System>> ListAllSystems();

  /**
   * @brief Checks if a system is enabled/disabled.
   *
   * @param system System UUID to check if enabled or disabled.
   *
   * @return True if enabled, false otherwise.
   */
  [[nodiscard("Pointless call of GetSystemEnable")]]
  bool GetSystemEnable(const System &system) noexcept;

  /**
   * @brief Set a system to be enabled/disabled.
   *
   * @param system System UUID to set to be enabled or disabled.
   * @param beEnabled True to enable, false to disable.
   */
  void SetSystemEnable(const System &system, bool beEnabled = true);

  /**
   * @brief Force run a system at any point of the execution.
   *
   * @param system System UUID to force run.
   * @param ignoreEnabled if set to true, this function
   * won't care if it is enabled or not. Otherwise it will
   * follow the enable/disable rule.
   */
  void InvokeSystem(const System &system, bool ignoreEnabled = true);

  /**
   * @brief Removes a system if exists.
   *
   * @param system System UUID to attempt to remove.
   *
   * @return True if system did exist and got destroyed, otherwise false.
   */
  bool RemoveSystem(const System &system);

  // ======== Components ========
  /**
   * @brief Adds a component to an entity.
   *
   * @tparam Component Any type that publically inherits
   * Tourmaline::Systems::ECS::Component.
   * @tparam ComponentArgs Arguments to be used for construction
   * of the component.
   *
   * @param entity Entity UUID to add the component.
   * @param args Arguments to construct the component.
   */
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

  /**
   * @brief Fetches specified component of an entity, if available.
   *
   * @tparam Component Any type that publically inherits
   * Tourmaline::Systems::ECS::Component.
   *
   * @param entity Entity UUID to fetch the component.
   *
   * @return A reference to the component requested.
   *
   * @warning If the component requested is not available the program will
   * crash! Unless you are absolutely sure, please use
   * Tourmaine::Systems::ECS::World::HasComponent to check
   * if the component exists.
   */
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

  /**
   * @brief Checks if an entity has a component.
   *
   * @tparam Component Any type that publically inherits
   * Tourmaline::Systems::ECS::Component.
   *
   * @param entity Entity UUID to check for the component.
   *
   * @return True if the component is available, false otherwise.
   */
  template <isAComponent Component>
  [[nodiscard("Pointless call of HasComponent")]]
  bool HasComponent(const Entity &entity) {
    return entityComponentMap.Query(entity, typeid(Component)).size();
  }

  /**
   * @brief Removes a component from an entity.
   *
   * @tparam Component Any type that publically inherits
   * Tourmaline::Systems::ECS::Component.
   *
   * @param entity Entity UUID to remove the component.
   *
   * @return True if the component was available and removed, false otherwise.
   */
  template <isAComponent Component> bool RemoveComponent(const Entity &entity) {
    return entityComponentMap.Remove(entity, typeid(Component));
  }

  /// @warning Copying is not allowed since the ECS world is meant to be
  /// a session with its own private session sensitive variables
  World(const World &) = delete;

  /// @warning Copying is not allowed since the ECS world is meant to be
  /// a session with its own private session sensitive variables
  World &operator=(const World &) = delete;

private:
  Containers::DualkeyMap<Entity, std::type_index, std::any>
      entityComponentMap{};
  Containers::Hashmap<Entity, Corrade::Containers::String> entityLabelList{};

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
