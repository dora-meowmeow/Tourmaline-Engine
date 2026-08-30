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
#include <cstddef>
#include <cstdlib>
#include <functional>
#include <memory>
#include <type_traits>
#include <typeindex>
#include <typeinfo>
#include <utility>
#include <vector>

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

/// @brief The priority with which a system will run. Start will run first and
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
   * @brief Runs each system for one time. Could be interpreted as a
   * "World Tick".
   */
  void Step();

  // ========  Entities  ========
  /**
   * @brief Creates an entity with a
   * Tourmaline::Systems::Components::Transform component.
   *
   * @param isEnabled Whether or not the entity should be enabled on creation.
   * @param presetUUID Useful if you need an entity to have a specific UUID.
   *
   * @return The created entity's UUID.
   */
  Entity CreateEntity(bool isEnabled = true, Type::UUID presetUUID = {0, 0});

  /**
   * @brief Creates an entity with
   * a Tourmaline::Systems::Components::Transform component, as well as the
   * components in a given Tourmaline::Systems::ECS::Prefab, with specific
   * values set per component.
   *
   * @tparam Components This field should be automatically filled by the
   * prefab itself.
   *
   * @param prefab The Prefab to use for the creation of the entity.
   * @param isEnabled Whether or not the entity should be enabled on creation.
   * @param presetUUID Useful if you need an entity to have a specific UUID.
   *
   * @return The created entity's UUID.
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
   * @brief Checks if an entity UUID is valid (the entity exists).
   *
   * @param entity Entity UUID to check.
   *
   * @return True if the entity exists, false otherwise.
   */
  [[nodiscard("Pointless call of EntityExists")]]
  bool EntityExists(const Entity &entity) noexcept;

  /**
   * @brief Destroys an entity (if it exists).
   *
   * @param entity Entity UUID of the entity to attempt to destroy.
   *
   * @return True if the entity existed and was successfully destroyed,
   * false otherwise.
   */
  bool DestroyEntity(Entity entity);

  /**
   * @brief Enables/disables an entity for use by a system.
   *
   * @param entity Entity UUID of the entity to enable/disable.
   * @param beEnabled True to enable, false to disable.
   */
  void SetEntityEnable(const Entity &entity, bool beEnabled = true) noexcept;

  /**
   * @brief Fetches whether an entity is enabled or disabled.
   *
   * @param entity Entity UUID of the entity to check.
   *
   * @return True if enabled, false if disabled.
   */
  [[nodiscard("Pointless call of GetEntityEnable")]]
  bool GetEntityEnable(const Entity &entity) noexcept;

  /**
   * @brief Sets a label/name on an entity.
   *
   * @param entity Entity UUID of the entity to be labeled.
   * @param label The label/name to be set.
   *
   */
  void SetEntityLabel(const Entity &entity, Corrade::Containers::String label);

  /**
   * @brief Fetches entity's label/name, if available (i.e. present).
   *
   * @param entity Entity UUID of the entity from which to fetch the label.
   *
   * @return If labeled, returns the labeled name. Otherwise, returns the string
   * "unknown".
   */
  [[nodiscard("Pointless call of GetEntityLabel")]]
  Corrade::Containers::StringView GetEntityLabel(const Entity &entity) noexcept;

  // ======== Systems ========
  /**
   * @brief Overload of AddSystem for convenience.
   *
   * This is an overload for the sake of convenience, for when specifying
   * a class instance. It doesn't do anything differently except
   * take a different order of arguments.
   */
  template <typename SystemFunction, typename Instance>
  System AddSystem(SystemFunction &&system, Instance *instance,
                   SystemPriority priority = Default, bool enabled = true) {
    return AddSystem(system, priority, enabled, instance);
  }

  /**
   * @brief Adds any function, provided it follows the rules specified below, as
   * a system.
   *
   * @tparam SystemFunction A function (could be a member function of a class)
   * which follows the rules specified below.
   * @tparam Instance If the function is a member function of a class, set this
   * as the class type. Otherwise leave it as is.
   *
   * @param system The function to add as a system.
   * @param priority Refer to Tourmaline::System::ECS::SystemPriority.
   * @param enabled Whether the system should be enabled from creation.
   * @param instance A pointer to a class instance. Required for member
   * functions of a class that AREN'T static (as context to run the function
   * in).
   *
   * @return The System UUID of the created system.
   *
   * ## A function must follow the following rules to be a system.
   * - The return type of the function must be void.
   * - Must have at least 2 arguments.
   *   - First argument must be of type const Entity& (or const
   * Tourmaline::Systems::ECS::Entity&).
   *   - Every argument except the first must be of a type that publicly
   * inherits Tourmaline::Systems::ECS::Component.
   * - If it is a pointer-to-member function (i.e. member function of a class),
   * there must be an instance of a class specified for the function to run in.
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
                  "A System is required to take at least 1 more "
                  "argument aside from const Entity&!");
    static_assert(std::is_same_v<firstArgument, const Entity &>,
                  "First Argument must be of the type const Entity&!");
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
          "Every argument aside from the first argument must be derived from "
          "ECS::Component");
    }(std::make_integer_sequence<std::size_t, componentCount>{});

    // Type erasure nightmare
    System newSystem = Random::GenerateUUID();
    systemFunction internalFunction =
        [system, instance,
         requiresInstance](const Entity &entity,
                           std::span<std::unique_ptr<ECS::Component> *> args) {
          [&]<std::size_t... index>(std::index_sequence<index...>) {
            // This check could be done in World::Step(), however
            // it is easier (and cheaper I believe) to implement it here
            if ((args[index]->get()->isEnabled && ...)) [[likely]] {

              // Pointer to Member functions
              if constexpr (requiresInstance) {
                (instance->*system)(
                    entity,
                    (static_cast<typename Traits::template argument<index + 1>>(
                        *args[index]->get()))...);
              } else {
                system(
                    entity,
                    (static_cast<typename Traits::template argument<index + 1>>(
                        *args[index]->get()))...);
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
   * @return A view of a list of vectors. There are in total 5 entries,
   * one per system priority level.
   * The first vector consists of the highest priority level systems,
   * and the remaining 4 vectors are ordered in decreasing priority level.
   *
   * @note Refer to Tourmaline::System::ECS::SystemPriority for
   * more information.
   */
  [[nodiscard("Pointless call of ListAllSystems")]]
  std::span<std::vector<System>> ListAllSystems();

  /**
   * @brief Checks if a system is enabled/disabled.
   *
   * @param system System UUID of the system to check.
   *
   * @return True if enabled, false otherwise.
   */
  [[nodiscard("Pointless call of GetSystemEnable")]]
  bool GetSystemEnable(const System &system) noexcept;

  /**
   * @brief Enables/disables a system.
   *
   * @param system System UUID of the system to enable/disable.
   * @param beEnabled True to enable, false to disable.
   */
  void SetSystemEnable(const System &system, bool beEnabled = true);

  /**
   * @brief Force run a system at any point of the execution.
   *
   * @param system System UUID of the system to force run.
   * @param ignoreEnabled if set to true, this function
   * won't care whether the system is enabled or not. Otherwise it will
   * follow the enable/disable rule of the system.
   */
  void InvokeSystem(const System &system, bool ignoreEnabled = true);

  /**
   * @brief Removes a system (if it exists).
   *
   * @param system System UUID of the system to attempt to remove.
   *
   * @return True if the system existed and was successfully destroyed,
   * false otherwise.
   */
  bool RemoveSystem(const System &system);

  // ======== Components ========
  /**
   * @brief Adds a component to an entity.
   *
   * @tparam Component Any type that publicly inherits
   * Tourmaline::Systems::ECS::Component.
   * @tparam ComponentArgs Types of the arguments to be used for the
   * construction of the component.
   *
   * @param entity Entity UUID of the entity to which the component will be
   * added.
   * @param args Arguments to construct the component.
   */
  template <isAComponent Component, typename... ComponentArgs>
  Component &AddComponent(const Entity &entity, ComponentArgs &&...args) {
    auto newComponent = entityComponentMap.Insert(
        entity, typeid(Component), std::make_unique<Component>(args...));
    if (componentCacheMap.Has(typeid(Component))) {
      for (systemCache *cache : componentCacheMap.Get(typeid(Component))) {
        cache->isStoring = false;
      }
    }

    return static_cast<Component &>(*std::get<2>(newComponent).get());
  }

  /**
   * @brief Fetches a specified component of an entity, if available (i.e.
   * present).
   *
   * @tparam Component Any type that publicly inherits
   * Tourmaline::Systems::ECS::Component.
   *
   * @param entity Entity UUID of the entity from which to fetch the component.
   *
   * @return A reference to the requested component.
   *
   * @warning If the requested component is not available, the program will
   * crash! Unless you are absolutely sure, please use
   * Tourmaline::Systems::ECS::World::HasComponent to check
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
    return static_cast<Component &>(*result.begin()->second.get());
  }

  /**
   * @brief Fetches every instance of a component, if available (i.e.
   * present).
   *
   * @tparam Component Any type that publicly inherits
   * Tourmaline::Systems::ECS::Component.
   *
   * @return A vector of every instance of specified the component and the
   * entity associated with it. Returns an empty vector if none exists.
   *
   * @warning This is a somewhat costly operation. It is suggested that you
   * cache the result.
   */
  template <isAComponent Component>
  [[nodiscard("Pointless call of GetAllComponentOfType")]]
  std::vector<std::pair<const Entity &, Component &>> GetAllOfComponents() {
    auto queryResult =
        entityComponentMap.Query(std::nullopt, typeid(Component));
    if (queryResult.empty()) {
      return {};
    }

    std::vector<std::pair<const Entity &, Component &>> result;
    for (const auto &pair : queryResult) {
      result.emplace_back(
          std::get<std::reference_wrapper<const Entity>>(pair.first).get(),
          static_cast<Component &>(*pair.second));
    }

    return result;
  }

  /**
   * @brief Checks if an entity has a component.
   *
   * @tparam Component Any type that publicly inherits
   * Tourmaline::Systems::ECS::Component.
   *
   * @param entity Entity UUID of the entity in which to check for the
   * component.
   *
   * @return True if the component is present, false otherwise.
   */
  template <isAComponent Component>
  [[nodiscard("Pointless call of HasComponent")]]
  bool HasComponent(const Entity &entity) {
    return entityComponentMap.Query(entity, typeid(Component)).size();
  }

  /**
   * @brief Removes a component from an entity.
   *
   * @tparam Component Any type that publicly inherits
   * Tourmaline::Systems::ECS::Component.
   *
   * @param entity Entity UUID of the entity from which to remove the component.
   *
   * @return True if the component was present and successfully removed, false
   * otherwise.
   *
   * @warning You cannot remove Tourmaline::Systems::Components::Transform using
   * this function.
   */
  template <isAComponent Component> bool RemoveComponent(const Entity &entity) {
    static_assert(!std::is_same_v<Component, Components::Transform>,
                  "Tried to remove Tourmaline::Systems::Components::Transform "
                  "from an entity. This is not allowed!");
    return entityComponentMap.Remove(entity, typeid(Component));
  }

  /// @warning Copying is not allowed, since the ECS world is meant to be
  /// a session with its own private session-sensitive variables.
  World(const World &) = delete;

  /// @warning Copying is not allowed, since the ECS world is meant to be
  /// a session with its own private session-sensitive variables.
  World &operator=(const World &) = delete;

private:
  Containers::DualkeyMap<Entity, std::type_index,
                         std::unique_ptr<ECS::Component>>
      entityComponentMap{};
  Containers::Hashmap<Entity, Corrade::Containers::String> entityLabelList{};

  // Systems
  using systemFunction = Corrade::Containers::Function<void(
      const Entity &, std::span<std::unique_ptr<ECS::Component> *>)>;
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
