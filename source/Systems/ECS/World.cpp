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
#include <algorithm>
#include <typeindex>

using namespace Tourmaline::Systems;
using namespace ECS;

void World::Step() {
  preSystems();

  for (const System &system : systemList) {
    systemStorage &storage = systemRegistry.Get(system);
    if (storage.isEnabled) {
      // It is being done here to eliminate repetitive code
      if (!storage.cache->isStoring) {
        storage.cache->storage =
            entityComponentMap.QueryWithAll<std::type_index>(
                storage.cache->arguments, true);
        storage.cache->isStoring = true;
      }

      for (componentCache &entry : storage.cache->storage) {
        if (entityEnableList.Get(*entry.oppositeKey)) [[likely]] {
          storage.function(*entry.oppositeKey, entry.valueQueryResults);
        }
      }
    }
  }

  postSystems();
}

void World::preSystems() {
  // Defined for future use
}

void World::postSystems() {
  // Defined for future use
}

std::span<System> World::ListAllSystems() { return systemList; }
bool World::GetSystemEnable(const System &system) noexcept {
  return systemRegistry.Has(system) && systemRegistry.Get(system).isEnabled;
}

void World::SetSystemEnable(const System &system, bool beEnabled) {
  if (!systemRegistry.Has(system)) {
    Logging::LogFormatted(
        "System {} does not exist therefore it cannot be set!",
        "ECS/SetSystemEnable", Logging::Warning, system.asString());
    return;
  }

  systemRegistry.Get(system).isEnabled = beEnabled;
}

bool World::DestroySystem(const System &system) {
  if (systemRegistry.Has(system)) {
    systemList.erase(std::find(systemList.begin(), systemList.end(), system));
    systemRegistry.Remove(system);
    return true;
  }

  Logging::LogFormatted("Tried to remove a non existend system with ID {}",
                        "ECS/DestroySystem", Logging::Warning,
                        system.asString());
  return false;
}

// Entities
Entity World::CreateEntity(bool beEnabled) {
  auto newEntity = Random::GenerateUUID();

  // Default components
  entityComponentMap.Insert(newEntity, typeid(Components::Transform),
                            Components::Transform());
  entityEnableList.Insert(newEntity, beEnabled);

  if (componentCacheMap.Has(typeid(Components::Transform))) {
    for (systemCache *cache :
         componentCacheMap.Get(typeid(Components::Transform))) {
      cache->isStoring = false;
    }
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

bool World::GetEntityEnable(const Entity &entity) noexcept {
  return entityEnableList.Has(entity) && entityEnableList.Get(entity);
}

void World::SetEntityEnable(const Entity &entity, bool beEnabled) {
  if (!entityEnableList.Has(entity)) {
    Logging::LogFormatted(
        "Entity {} does not exist therefore it cannot be set!",
        "ECS/SetEntityEnable", Logging::Warning, entity.asString());
    return;
  }

  entityEnableList.Get(entity) = beEnabled;
}

bool World::DestroyEntity(Entity entity) {
  if (!entityComponentMap.Remove(entity, std::nullopt)) [[unlikely]] {
    Logging::LogFormatted("Trying to delete Entity {} that does not exist!",
                          "ECS/DestroyEntity", Logging::Warning,
                          entity.asString());
    return false;
  }

  entityEnableList.Remove(entity);
  return true;
}
