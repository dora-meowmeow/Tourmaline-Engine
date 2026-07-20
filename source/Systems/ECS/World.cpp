/*
 * SPDX-FileCopyrightText: Dora "cat" <cat@thenight.club>
 * SPDX-License-Identifier: MPL-2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "Systems/ECS.hpp"
#include "Systems/Logging.hpp"

#include <algorithm>
#include <cstdint>
#include <typeindex>
#include <vector>

using namespace Tourmaline::Systems;
using namespace ECS;

void World::Step() {
  preSystems();

  for (uint8_t priority = SystemPriority::Start;
       priority < SystemPriority::Final; priority++) {
    for (const System &system : systemList[priority]) {
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
          if (!disabledEntityList.Has(*entry.oppositeKey)) [[likely]] {
            storage.function(*entry.oppositeKey, entry.valueQueryResults);
          }
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

std::span<std::vector<System>> World::ListAllSystems() { return systemList; }
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

bool World::RemoveSystem(const System &system) {
  if (systemRegistry.Has(system)) {
    SystemPriority priority = systemRegistry.Get(system).priority;
    systemList[priority].erase(std::find(systemList[priority].begin(),
                                         systemList[priority].end(), system));
    systemRegistry.Remove(system);
    return true;
  }

  Logging::LogFormatted("Tried to remove a non existend system with ID {}",
                        "ECS/DestroySystem", Logging::Warning,
                        system.asString());
  return false;
}
