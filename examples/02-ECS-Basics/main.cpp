/*
 * SPDX-FileCopyrightText: Dora "cat" <cat@thenight.club>
 * SPDX-License-Identifier: MPL-2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <Tourmaline/Systems/ECS.hpp>
#include <Tourmaline/Systems/ECS/BuiltinComponents.hpp>
#include <Tourmaline/Types.hpp>

using namespace Tourmaline;
using namespace Tourmaline::Systems;

// Every component must publicly inherit ECS::Component
struct Stats : public ECS::Component {
  double HP = 100, maxHP = 100;
  double mana = 200, maxMana = 200;
};

int main() {
  // This is the main class that owns the entire ECS system.
  ECS::World world;

  // You can create entities with this function.
  // It will return you an entity UUID that is randomly generated.
  ECS::Entity entity = world.CreateEntity();

  // If you need to you can also specify if it should start enabled and
  // it's UUID.
  ECS::Entity player =
      world.CreateEntity(false, Type::UUID("6B62E1F98D234BE92E6D93A3C7B0FDCA"));

  // Each ECS::Entity will have a default component of
  // Tourmaline::Systems::Components::Transform.
  //
  // Reference are designed to NEVER go stale. So feel free to cache them and
  // reuse them.
  Components::Transform &playerTransform =
      world.GetComponent<Components::Transform>(player);

  // Changes done will directly effect the component itself.
  playerTransform.Translate = {3, 0.5, -10.2};

  // We can add our custom component like so.
  //
  // First two arguments are always the same, which entity to add and if it
  // should be enabled. The rest of the arguments will be forwarded to construct
  // the class.
  //
  // So here our player will have 200 HP and Max HP,
  // while entity will have 100 (default).
  world.AddComponent<Stats>(player, true, 200, 200);

  // AddComponent will return a reference to the added component so you don't
  // have to fetch it.
  Stats &entitysStats = world.AddComponent<Stats>(entity, true);

  // Every component will have a isEnabled member that you can use to
  // enable/disable them.
  entitysStats.isEnabled = false;

  // We can enable/disable any entity we want with this function.
  //
  // This function has a get variant named GetEntityEnable
  world.SetEntityEnable(player, true);

  // We also can label each entity to know what UUID stands for what.
  //
  // This function has a get variant named GetEntityLabel
  world.SetEntityLabel(player, "Player Character");

  // Entities with no label set will automatically be labeled as "unknown".
  auto entityName = world.GetEntityLabel(entity);
  Logging::Log(entityName, "Game");

  // We can destroy entities with this function.
  //
  // It is advised to check if the entity exists, before destroying an entity.
  // This isn't a requirement but will save you from a lot of headaches.
  if (world.EntityExists(entity)) {
    // You can also check if the entity was successfully deleted using the
    // returned value.
    bool isDeleted = world.DestroyEntity(entity);
    Logging::LogFormatted("Is entity deleted successfully? {}", "Game",
                          Logging::Info, isDeleted);
  }

  // Likewise we can delete components (except
  // Tourmaline::Systems::Components::Transform)
  //
  // As said before it is highly advised to see if a component exists before
  // destroying.
  if (world.HasComponent<Stats>(player)) {
    // You can also check if the component was successfully deleted using the
    // returned value.
    bool isDeleted = world.RemoveComponent<Stats>(player);
    Logging::LogFormatted(
        "Is player's Stats component deleted successfully? {}", "Game",
        Logging::Info, isDeleted);
  }

  // The ECS system supports Prefabs and Systems, we talk more about them in
  // ECS-Advanced example.
}
