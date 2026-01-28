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

using namespace Tourmaline::Systems::ECS;

// It is preferable to send a copy of the UUID instead of reference since
// the entity itself may be destroyed in the memory
Entity World::CreateEntity() {
  auto newEntity = entityComponentMap.insert(
      Random::GenerateUUID(), typeid(Tourmaline::Systems::Components::Position),
      Tourmaline::Systems::Components::Position());

  return Entity(std::get<0>(newEntity));
}

bool World::EntityExists(const Entity &entity) noexcept {
  // TO BE IMPLEMENTED
  return true;
}

bool World::DestroyEntity(Entity entity) {
  return entityComponentMap.remove(entity, std::nullopt);
}
