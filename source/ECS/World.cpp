/*
 * SPDX-FileCopyrightText: Dora "cat" <cat@thenight.club>
 * SPDX-License-Identifier: MPL-2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "../../headers/ECS.hpp"
#include "../../headers/Systems/Random.hpp"

using namespace Tourmaline::ECS;

Entity World::CreateEntity() {
  auto [iterator, success] =
      entityComponentList.try_emplace(Systems::Random::GenerateUUID());

  Systems::Logging::Log("Failed to create an entity! Possibly by incredible "
                        "luck generated already existing UUID?",
                        "CreateEntity", Systems::Logging::LogLevel::Critical,
                        !success);
  return iterator->first;
}

bool World::EntityExists(const Entity &entity) noexcept {
  return entityComponentList.find(entity) != entityComponentList.end();
}

bool World::DestroyEntity(Entity entity) {
  auto entityIter = GetEntityIterator(
      entity,
      std::format("Cannot delete entity \"{}\", it does not exist!",
                  entity.asString()),
      "DestroyEntity");

  if (entityIter == entityComponentList.end()) {
    return false;
  }

  entityComponentList.erase(entityIter);
  return true;
}

// Very repetitive code
decltype(World::entityComponentList)::iterator
World::GetEntityIterator(const Entity &entity, const std::string &errorMessage,
                         const std::string &position,
                         Tourmaline::Systems::Logging::LogLevel severity) {
  auto iter = entityComponentList.find(entity);

  Systems::Logging::Log(errorMessage, "GetEntityIterator/" + position, severity,
                        iter == entityComponentList.end());
  return iter;
}
