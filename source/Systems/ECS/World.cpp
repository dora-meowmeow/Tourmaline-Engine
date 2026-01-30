/*
 * SPDX-FileCopyrightText: Dora "cat" <cat@thenight.club>
 * SPDX-License-Identifier: MPL-2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <Systems/ECS.hpp>
#include <Systems/ECS/BuiltinComponents.hpp>
#include <Systems/Random.hpp>
#include <optional>

using namespace Tourmaline::Systems;
using namespace ECS;

void World::Step() {
  preSystems();
  // Actual systems will happen here
  postSystems();
}

void World::preSystems() {
  // Defined for future use
}

void World::postSystems() {
  // Can't do a foreach with a std::stack
  while (!entitiesToDisable.empty()) {
    std::pair<Components::Enabled *, bool> &request = entitiesToDisable.top();
    request.first->enabled = request.second;
    entitiesToDisable.pop();
  }
}

// Entities
Entity World::CreateEntity() {
  auto newEntity = Random::GenerateUUID();

  // Default components
  entityComponentMap.insert(newEntity, typeid(Components::Position),
                            Components::Position());
  entityComponentMap.insert(newEntity, typeid(Components::Enabled),
                            Components::Enabled(this));

  return newEntity;
}

const Components::Enabled &World::EntityEnable(const Entity &entity) noexcept {
  return this->GetComponent<Components::Enabled>(entity);
}

bool World::EntityExists(const Entity &entity) noexcept {
  bool exists = false;
  entityComponentMap.scan(
      [&exists, entity](const Tourmaline::Type::UUID &currentEntity,
                        const std::type_index &, std::any &) -> bool {
        if (currentEntity == entity) {
          exists = true;
          return true;
        }
        return false;
      });
  return exists;
}

bool World::DestroyEntity(Entity entity) {
  return entityComponentMap.remove(entity, std::nullopt);
}
