/*
 * SPDX-FileCopyrightText: Dora "cat" <cat@thenight.club>
 * SPDX-License-Identifier: MPL-2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "Systems/ECS.hpp"
#include "Systems/ECS/BuiltinComponents.hpp"
#include "Systems/Logging.hpp"
#include "Systems/Random.hpp"

using namespace Tourmaline::Systems;
using namespace ECS;

// Entities
Entity World::CreateEntity(bool isEnabled) {
  auto newEntity = Random::GenerateUUID();

  // Default components
  entityComponentMap.Insert(newEntity, typeid(Components::Transform),
                            Components::Transform());

  if (componentCacheMap.Has(typeid(Components::Transform))) {
    for (systemCache *cache :
         componentCacheMap.Get(typeid(Components::Transform))) {
      cache->isStoring = false;
    }
  }

  if (!isEnabled) {
    SetEntityEnable(newEntity, isEnabled);
  }

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

void World::SetEntityEnable(const Entity &entity, bool beEnabled) noexcept {
  bool isDisabled = disabledEntityList.Has(entity);

  // Disabled - Disable/Enabled - Enable case
  if (isDisabled ^ beEnabled) {
    Logging::LogFormatted(
        "Trying to set entity {} to be {}, when it already is.",
        "ECS/SetEntityEnable", Logging::Warning, entity.asString(),
        beEnabled ? "Enabled" : "Disabled");
    return;
  }

  if (beEnabled) {
    disabledEntityList.Remove(entity);
    return;
  }
  disabledEntityList.Insert(entity);
}

bool World::GetEntityEnable(const Entity &entity) noexcept {
  return disabledEntityList.Has(entity);
}

bool World::DestroyEntity(Entity entity) {
  return entityComponentMap.Remove(entity, std::nullopt);
}
