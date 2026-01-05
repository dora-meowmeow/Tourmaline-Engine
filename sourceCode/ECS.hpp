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
#include <concepts>
#include <format>
#include <typeindex>
#include <unordered_map>
#include <utility>

#include "Systems/Logging.hpp"
#include "Types.hpp"

namespace Tourmaline::ECS {
using Entity = Tourmaline::Type::UUID;
class World;

struct BaseComponent {
public:
  virtual ~BaseComponent() = default;
  const Entity &GetOwner();

private:
  const Entity *owner;
  friend World;
};

// Concepts
template <typename T>
concept Component = std::derived_from<T, BaseComponent>;

class World {
public:
  // Entity
  Entity CreateEntity();
  bool EntityExists(const Entity &entity) noexcept;
  [[nodiscard("It is not guaranteed that an entity can always be destroyed, "
              "please make "
              "sure by checking the returned bool")]]
  bool DestroyEntity(Entity entity);

  // Components
  template <Component T, typename... Args>
  T &AddComponent(const Entity &entity, Args &&...constructionArguments) {
    // Insert to entity list
    auto entityIter = GetEntityIterator(
        entity,
        std::format(
            "Cannot add component \"{}\"! Entity \"{}\" does not exist!",
            typeid(T).name(), entity.asString()),
        "AddComponent", Systems::Logging::LogLevel::Error);

    auto [componentIter, success] = entityIter->second.try_emplace(
        typeid(T), T(std::forward<Args>(constructionArguments)...));
    Systems::Logging::Log(
        std::format("Cannot add component! Component \"{}\" already exists "
                    "in entity \"{}\" ",
                    typeid(T).name(), entity.asString()),
        "AddComponent", Systems::Logging::LogLevel::Error, !success);

    T &component = std::any_cast<T &>(componentIter->second);
    component.owner = &entity;
    return component;
  }

  template <Component T>
  [[nodiscard("Discarding an expensive operation's result!")]]
  T &GetComponent(const Entity &entity) {
    auto iter = GetEntityIterator(
        entity,
        std::format("Can't get entity \"{}\"'s component \"{}\", since "
                    "entity does not exist!",
                    entity.asString(), typeid(T).name()),
        "GetComponent", Systems::Logging::LogLevel::Error);

    auto component = iter->second.find(typeid(T));
    Systems::Logging::Log(
        std::format(
            "Entity \"{}\" does not have component \"{}\", cannot get it!",
            entity.asString(), typeid(T).name()),
        "GetComponent", Systems::Logging::LogLevel::Error,
        component == iter->second.end());

    return std::any_cast<T &>(component->second);
  }

  template <Component T>
  [[nodiscard("Discarding an expensive operation's result!")]]
  bool HasComponent(const Entity &entity) {
    auto iter = GetEntityIterator(
        entity,
        std::format("Can't find if entity \"{}\" has component \"{}\", since "
                    "entity does not exist!",
                    entity.asString(), typeid(T).name()));

    return iter != entityComponentList.end() &&
           (iter->second.find(typeid(T)) != iter->second.end());
  }

  template <Component T>
  [[nodiscard("It is not guaranteed that a component can always be removed, "
              "please make "
              "sure by checking the returned bool")]]
  bool RemoveComponent(const Entity &entity) {
    auto entityIter = GetEntityIterator(
        entity,
        std::format("Cannot remove component {} from entity {}, since entity "
                    "does not exist!",
                    typeid(T).name(), entity.asString()),
        "RemoveComponent", Systems::Logging::LogLevel::Warning);
    if (entityIter == entityComponentList.end()) {
      return false;
    }

    auto componentIter = entityIter->second.find(typeid(T));
    if (componentIter == entityIter->second.end()) {
      Systems::Logging::Log(
          std::format("Cannot remove component {} from entity {}, since entity "
                      "does not have that component",
                      typeid(T).name(), entity.asString()),
          "RemoveComponent", Systems::Logging::LogLevel::Warning);
      return false;
    }

    entityIter->second.erase(componentIter);
    return true;
  }

private:
  std::unordered_map<Entity, std::unordered_map<std::type_index, std::any>>
      entityComponentList{};

  decltype(entityComponentList)::iterator
  GetEntityIterator(const Entity &entity, const std::string &errorMessage = "",
                    const std::string &position = "",
                    Tourmaline::Systems::Logging::LogLevel severity =
                        Systems::Logging::LogLevel::Warning);
};
} // namespace Tourmaline::ECS
#endif
