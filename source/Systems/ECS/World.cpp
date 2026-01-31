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
  // Defined for future use
}

// Entities
Entity World::CreateEntity() {
  auto newEntity = Random::GenerateUUID();

  // Default components
  entityComponentMap.Insert(newEntity, typeid(Components::Base),
                            Components::Base());

  return newEntity;
}

bool World::EntityExists(const Entity &entity) noexcept {
  bool exists = false;
  entityComponentMap.Scan(
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
  return entityComponentMap.Remove(entity, std::nullopt);
}
