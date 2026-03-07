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
#include <typeindex>

#include "../Containers/DualkeyMap.hpp"
#include "../Containers/Hashmap.hpp"
#include "../Types.hpp"
#include "ECS/BuiltinComponents.hpp"
#include "Logging.hpp"

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
                            "ECS/GetComponent", Logging::LogLevel::Error,
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
  using systemFunction =
      std::function<void(const Entity &, std::span<std::any *>)>;
  Containers::DualkeyMap<Entity, std::type_index, std::any>
      entityComponentMap{};
  Containers::Hashmap<System, systemFunction> registeredSystems{};

  // ======== Life-cycle ========
  void preSystems();
  void postSystems();
};
} // namespace Tourmaline::Systems::ECS
#endif
