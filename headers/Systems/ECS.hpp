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

#include "../Containers/DualkeyMap.hpp"
#include "../Types.hpp"
#include "Logging.hpp"

namespace Tourmaline::Systems::ECS {
using Entity = Tourmaline::Type::UUID;
class World;

struct BaseComponent {
public:
  virtual ~BaseComponent() = default;

private:
  friend World;
};

// Concepts
template <typename T>
concept Component = std::derived_from<T, BaseComponent>;

class World {
public:
  // Entity
  [[nodiscard]]
  Entity CreateEntity();
  bool EntityExists(const Entity &entity) noexcept;
  [[nodiscard("It is not guaranteed that an entity can always be destroyed, "
              "please make sure by checking the returned bool")]]
  bool DestroyEntity(Entity entity);

  // Components
  template <Component component, typename... Args>
  component &AddComponent(const Entity &entity,
                          Args &&...constructionArguments) {
    auto newComponent = entityComponentMap.insert(
        entity, typeid(component), component(constructionArguments...));

    return std::any_cast<component &>(std::get<2>(newComponent));
  }

  template <Component component>
  [[nodiscard("Discarding an expensive operation's result!")]]
  component &GetComponent(const Entity &entity) {
    auto result = entityComponentMap.query(entity, typeid(component));
    if (result.empty()) {
      Logging::Log(std::format("Entity {} does not have component {}!",
                               entity.asString(), typeid(component).name()),
                   "ECS/GetComponent", Logging::LogLevel::Error);
    }
    return std::any_cast<component &>(result.begin()->second);
  }

  template <Component component>
  [[nodiscard("Discarding an expensive operation's result!")]]
  bool HasComponent(const Entity &entity) {
    // TO BE IMPLEMENTED
    return true;
  }

  template <Component component>
  [[nodiscard("It is not guaranteed that a component can always be removed, "
              "please make sure by checking the returned bool")]]
  bool RemoveComponent(const Entity &entity) {
    return entityComponentMap.remove(entity, typeid(component));
  }

private:
  Tourmaline::Containers::DualkeyMap<Entity, std::type_index, std::any>
      entityComponentMap{};
};
} // namespace Tourmaline::Systems::ECS
#endif
