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
#include "Types/Matrix.hpp"
#include <memory>

using namespace Tourmaline::Systems;
using namespace ECS;

// Entities
Entity World::CreateEntity(const Components::Transform &transformMatrix,
                           bool isEnabled, Type::UUID presetUUID) {
  bool isUnspecified = presetUUID == Type::UUID::Empty;
  auto newEntity = isUnspecified ? Random::GenerateUUID() : presetUUID;

  // In case someone is astronomically unlucky
  bool warned = false;
  while (EntityExists(newEntity)) {
    if (!isUnspecified && !warned) {
      Logging::LogFormatted(
          "Specified UUID {} already exists, generating a new UUID...",
          "ECS/CreateEntity", Logging::Warning, newEntity.asString());
      warned = true;
    }

    newEntity = Random::GenerateUUID();
  }

  // Default components
  auto newComponent = entityComponentMap.Insert(
      newEntity, typeid(Components::Transform),
      std::make_unique<Components::Transform>(transformMatrix));
  ECS::Component *componentPointer = std::get<2>(newComponent).get();
  triggerHooks<Components::Transform>(newEntity, componentPointer,
                                      HookTo::Creation);

  refreshAndInvalidateCaches<Components::Transform>();

  if (!isEnabled) {
    SetEntityEnable(newEntity, isEnabled);
  }
  usedEntityUUIDs.Insert(newEntity);

  return newEntity;
}

Entity World::CreateEntity(bool isEnabled, Type::UUID presetUUID) {
  static Type::Matrix defaultLocation;
  return CreateEntity(defaultLocation, isEnabled, presetUUID);
}

bool World::EntityExists(const Entity &entity) noexcept {
  return usedEntityUUIDs.Has(entity);
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

void World::SetEntityLabel(const Entity &entity,
                           Corrade::Containers::String label) {
  if (entityLabelList.Has(entity)) {
    entityLabelList.Get(entity) = label;
    return;
  }
  entityLabelList.Insert(entity, label);
}

Corrade::Containers::StringView
World::GetEntityLabel(const Entity &entity) noexcept {
  if (entityLabelList.Has(entity)) {
    return entityLabelList.Get(entity);
  }

  static Corrade::Containers::String unknown = "unknown";
  return unknown;
}

bool World::DestroyEntity(Entity entity) {
  // TODO: Hook here runs even if the destruction fails.
  // TODO: Hook here does not run for **every** component of an entity. Doing a
  // loop of RemoveComponent would be incredibly costly!
  triggerHooks<Components::Transform>(entity, HookTo::Destruction);
  size_t result = entityComponentMap.Remove(entity, std::nullopt);
  if (result) {
    refreshAndInvalidateCaches<Components::Transform>();
    usedEntityUUIDs.Remove(entity);
  }

  return result;
}
