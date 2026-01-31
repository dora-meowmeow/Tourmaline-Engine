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
#include <format>
#include <typeindex>

#include "../Containers/DualkeyMap.hpp"
#include "../Types.hpp"
#include "ECS/BuiltinComponents.hpp"
#include "Logging.hpp"

namespace Tourmaline::Systems::ECS {
using Entity = Tourmaline::Type::UUID;
class World;

class World {
public:
  // ====== World controls ======
  void Step();

  // ========  Entities  ========
  [[nodiscard]]
  Entity CreateEntity();
  bool EntityExists(const Entity &entity) noexcept;
  [[nodiscard("It is not guaranteed that an entity can always be destroyed, "
              "please make sure by checking the returned bool")]]
  bool DestroyEntity(Entity entity);

  // ======== Components ========
  template <isAComponent component, typename... Args>
  component &AddComponent(const Entity &entity,
                          Args &&...constructionArguments) {
    auto newComponent = entityComponentMap.insert(
        entity, typeid(component), component(constructionArguments...));

    return std::any_cast<component &>(std::get<2>(newComponent));
  }

  template <isAComponent component>
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

  template <isAComponent component>
  [[nodiscard("Discarding an expensive operation's result!")]]
  bool HasComponent(const Entity &entity) {
    return entityComponentMap.query(entity, typeid(component)).size();
  }

  template <isAComponent component>
  [[nodiscard("It is not guaranteed that a component can always be removed, "
              "please make sure by checking the returned bool")]]
  bool RemoveComponent(const Entity &entity) {
    return entityComponentMap.remove(entity, typeid(component));
  }

  // Copying is not allowed since the ECS world is meant to be
  // a session with its own private session sensitive variables
  World(const World &) = delete;
  World &operator=(const World &) = delete;

private:
  Tourmaline::Containers::DualkeyMap<Entity, std::type_index, std::any>
      entityComponentMap{};

  // ======== Life-cycle ========
  void preSystems();
  void postSystems();
};
} // namespace Tourmaline::Systems::ECS
#endif
