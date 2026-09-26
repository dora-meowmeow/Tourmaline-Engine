\page 04-ECS-Basics 04. ECS Basics - Entities & Components

Tourmaline defines all of its ECS functions under `Tourmaline/Systems/ECS.hpp`. So we will start by including said header.

(for convenience sake, we will use namespace `Tourmaline` and `Tourmaline::Systems`)

```c++
#include <Tourmaline/Systems/ECS.hpp>

using namespace Tourmaline;
using namespace Tourmaline::Systems;

int main(){

  return 0;
}
```

# What is ECS?

The Entity Component System (shortened to ECS) is a programming pattern made out of 3 distinct parts. Many game engines support ECS as it maps nicely to game logic. So we suggest that you familiarise yourself with the concept.

## Entities

Entities have a sole job of owning components. They do not store any data by themselves other than what components they own. Each entity can only own one copy of each component, so, for example, an entity cannot own two copies of a `Health` component. They are usually given an ID of some sort. Tourmaline defines each entity as a \ref Tourmaline::Type::UUID.

## Components

Components are structs that store information. They do not store any class/struct specific code. Their entire job is to store data and be owned by entities. Tourmaline defines each component as a \ref Tourmaline::Systems::ECS::Component.

## Systems

Systems are the functions that run on components owned by entities. They all have a certain list of components they need to run. For example: A system can request to run on every component `Transform` that is owned by an entity.

Additionally, systems may request to run on multiple components if they are __**both**__ owned by the same entity. For example: A system can request to run on any entity that has the components `isEnemy` and `Transform`. This means this system will ONLY run on each entity that __**both**__ owns `isEnemy` and `Transform`.

In Tourmaline a system is any function that satisfies the following requirements:

\htmlinclude SystemTypeRequirements.html

## Other important concepts

### World

World is the main class that owns every Entity, Component, and System. You will be interacting with the world to use the ECS. In Tourmaline this is done with the class \ref Tourmaline::Systems::ECS::World.

### Prefabs

Prefabs allow you to define an entity and which components it should have ahead of time. This allows you to make a prefab for, say, an enemy, include all of its components and set all of its values. Later on when you need to make said enemy, you can just use the prefab to make it. In Tourmaline this is done with the class \ref Tourmaline::Systems::ECS::Prefab in `Tourmaline/Systems/ECS/Prefab.hpp`.

# Creating the world

Now that we clarified the terminology and what an ECS is, we can start by creating a world. This is a pretty straight-forward procedure. We just construct the class \ref Tourmaline::Systems::ECS::World.

```c++
#include <Tourmaline/Systems/ECS.hpp>

using namespace Tourmaline;
using namespace Tourmaline::Systems;

int main(){
  // This is the main class that owns the entire ECS system.
  ECS::World world;
  return 0;
}
```

By itself the world won't do anything. However, it will be very useful as we start adding Entities and Components.

# Adding entities

You can add entities by using the \ref Tourmaline::Systems::ECS::World::CreateEntity function. If you ever want to reference that function, it is suggested that you store the returned \ref Tourmaline::Type::UUID.

```c++
#include <Tourmaline/Systems/ECS.hpp>

using namespace Tourmaline;
using namespace Tourmaline::Systems;

int main() {
  ECS::World world;
  // You can create entities with this function.
  // It will return you an entity UUID that is randomly generated.
  ECS::Entity entity = world.CreateEntity();

  // If you need to, you can also specify whether it should start enabled,
  // as well as its UUID.
  ECS::Entity player =
      world.CreateEntity(false, Type::UUID("6B62E1F98D234BE92E6D93A3C7B0FDCA"));

  return 0;
}
```

# Getting components

By default, every entity will have the \ref Tourmaline::Systems::Components::Transform component. This component __cannot__ be deleted. We can get a component owned by an entity by using \ref Tourmaline::Systems::ECS::World::GetComponent. This function will return a reference to the component which you can freely modify. The reference will __never__ go stale during its lifetime, so if you find yourself getting the same component over and over again. It would be wise to cache it. Systems by default cache each component they fetch.

```c++
#include <Tourmaline/Systems/ECS.hpp>

using namespace Tourmaline;
using namespace Tourmaline::Systems;

int main() {
  ECS::World world;

  ECS::Entity entity = world.CreateEntity();
  ECS::Entity player =
      world.CreateEntity(false, Type::UUID("6B62E1F98D234BE92E6D93A3C7B0FDCA"));

  // Each ECS::Entity will have a default component of
  // Tourmaline::Systems::Components::Transform.
  //
  // References are designed to NEVER go stale. So feel free to cache them and
  // reuse them.
  Components::Transform &playerTransform =
      world.GetComponent<Components::Transform>(player);

  // Changes done will directly affect the component itself.
  playerTransform.Translate = {3, 0.5, -10.2};

  return 0;
}
```

# Creating custom components

## Defining a custom component

Components are defined as any struct that inherits \ref Tourmaline::Systems::ECS::Component and contains members that store data. There is no rule on what members a component can contain, a component may have a member that is a pointer or reference.

```c++
#include <Tourmaline/Systems/ECS.hpp>

using namespace Tourmaline;
using namespace Tourmaline::Systems;

// Every component must publicly inherit ECS::Component.
struct Stats : public ECS::Component {
  double HP = 100, maxHP = 100;
  double mana = 200, maxMana = 200;
};

int main() {
  ECS::World world;

  ECS::Entity entity = world.CreateEntity();
  ECS::Entity player =
      world.CreateEntity(false, Type::UUID("6B62E1F98D234BE92E6D93A3C7B0FDCA"));

  Components::Transform &playerTransform =
      world.GetComponent<Components::Transform>(player);

  playerTransform.Translate = {3, 0.5, -10.2};

  return 0;
}
```

## Adding a custom component to an entity

You can add a new component to an entity by using \ref Tourmaline::Systems::ECS::World::AddComponent. It will return, if successful, the reference to the newly created component. This reference will __**never**__ go stale during its lifetime, so you can use it for as long as you desire.

```c++
#include <Tourmaline/Systems/ECS.hpp>

using namespace Tourmaline;
using namespace Tourmaline::Systems;

struct Stats : public ECS::Component {
  double HP = 100, maxHP = 100;
  double mana = 200, maxMana = 200;
};

int main() {
  ECS::World world;

  ECS::Entity entity = world.CreateEntity();
  ECS::Entity player =
      world.CreateEntity(false, Type::UUID("6B62E1F98D234BE92E6D93A3C7B0FDCA"));

  Components::Transform &playerTransform =
      world.GetComponent<Components::Transform>(player);

  playerTransform.Translate = {3, 0.5, -10.2};

  // The first two arguments are always the same: Which entity to add the
  // component to, and whether it should be enabled on creation. The remaining
  // arguments will be forwarded to construct the class.
  //
  // In this example, our player entity will have 200 HP and 200 Max HP as the
  // values of its Stats component, while our other entity (entity) will have
  // the default values of 100 (On the second function call, below this one), as
  // no component arguments are passed.
  world.AddComponent<Stats>(player, true, 200, 200);

  // AddComponent will return a reference to the added component so you don't
  // have to fetch it.
  Stats &entitysStats = world.AddComponent<Stats>(entity, true);

  return 0;
}
```

# Enabling/disabling entities and components

While currently not useful for this example, it is important to know for the ECS Advanced example how to enable and disable entities and components. For disabling entities we can use \ref Tourmaline::Systems::ECS::World::SetEntityEnable, for components you can just set `.isEnabled` to `true` or `false`.

```c++
#include <Tourmaline/Systems/ECS.hpp>

using namespace Tourmaline;
using namespace Tourmaline::Systems;

struct Stats : public ECS::Component {
  double HP = 100, maxHP = 100;
  double mana = 200, maxMana = 200;
};

int main() {
  ECS::World world;

  ECS::Entity entity = world.CreateEntity();
  ECS::Entity player =
      world.CreateEntity(false, Type::UUID("6B62E1F98D234BE92E6D93A3C7B0FDCA"));

  Components::Transform &playerTransform =
      world.GetComponent<Components::Transform>(player);

  playerTransform.Translate = {3, 0.5, -10.2};

  world.AddComponent<Stats>(player, true, 200, 200);
  Stats &entitysStats = world.AddComponent<Stats>(entity, true);

  // Every component has the attribute isEnabled, which you can use to
  // enable/disable it.
  entitysStats.isEnabled = false;

  // We can enable/disable any entity we want with this function.
  //
  // This function has a get variant named GetEntityEnable.
  world.SetEntityEnable(player, true);

  return 0;
}
```

# Putting labels on entities

It can be quite beneficial to know which entity UUID stands for which entity. This can be done by using \ref Tourmaline::Systems::ECS::World::SetEntityLabel. You can then also get the label of an entity by using \ref Tourmaline::Systems::ECS::World::GetEntityLabel.

```c++
#include <Tourmaline/Systems/ECS.hpp>

using namespace Tourmaline;
using namespace Tourmaline::Systems;

struct Stats : public ECS::Component {
  double HP = 100, maxHP = 100;
  double mana = 200, maxMana = 200;
};

int main() {
  ECS::World world;

  ECS::Entity entity = world.CreateEntity();
  ECS::Entity player =
      world.CreateEntity(false, Type::UUID("6B62E1F98D234BE92E6D93A3C7B0FDCA"));

  Components::Transform &playerTransform =
      world.GetComponent<Components::Transform>(player);

  playerTransform.Translate = {3, 0.5, -10.2};

  world.AddComponent<Stats>(player, true, 200, 200);
  Stats &entitysStats = world.AddComponent<Stats>(entity, true);

  entitysStats.isEnabled = false;
  world.SetEntityEnable(player, true);

  // We also can label each entity to know which UUIDs stand for what.
  //
  // This function has a get variant named GetEntityLabel.
  world.SetEntityLabel(player, "Player Character");

  // Entities with no label set will automatically be labeled as "unknown".
  auto entityName = world.GetEntityLabel(entity);
  auto playerName = world.GetEntityLabel(player);
  Logging::Log(entityName, "Game");
  Logging::Log(playerName, "Game");

  return 0;
}
```

Output:

```bash
 [Info@Game] unknown
 [Info@Game] Player Character
```

# Checking and destroying entities

Just as you can create entities, you can also destroy them by using \ref Tourmaline::Systems::ECS::World::DestroyEntity. However it is __strongly__ encouraged that you first check if the entity you are trying to destroy still exists by using \ref Tourmaline::Systems::ECS::World::EntityExists.

```c++
#include <Tourmaline/Systems/ECS.hpp>

using namespace Tourmaline;
using namespace Tourmaline::Systems;

struct Stats : public ECS::Component {
  double HP = 100, maxHP = 100;
  double mana = 200, maxMana = 200;
};

int main() {
  ECS::World world;

  ECS::Entity entity = world.CreateEntity();
  ECS::Entity player =
      world.CreateEntity(false, Type::UUID("6B62E1F98D234BE92E6D93A3C7B0FDCA"));

  Components::Transform &playerTransform =
      world.GetComponent<Components::Transform>(player);

  playerTransform.Translate = {3, 0.5, -10.2};

  world.AddComponent<Stats>(player, true, 200, 200);
  Stats &entitysStats = world.AddComponent<Stats>(entity, true);

  entitysStats.isEnabled = false;
  world.SetEntityEnable(player, true);

  world.SetEntityLabel(player, "Player Character");
  auto entityName = world.GetEntityLabel(entity);
  auto playerName = world.GetEntityLabel(player);
  Logging::Log(entityName, "Game");
  Logging::Log(playerName, "Game");

  // We can destroy entities with this function.
  //
  // It is advised to check if the entity exists, before destroying it.
  // This isn't a requirement, but it will save you from a lot of headaches.
  if (world.EntityExists(entity)) {
    // You can also check if the entity was successfully deleted using the
    // returned value. True if so, and false if deletion was unsuccessful.
    bool isDeleted = world.DestroyEntity(entity);
    Logging::LogFormatted("Is entity deleted successfully? {}", "Game",
                          Logging::Info, isDeleted);
  }

  return 0;
}
```

Output:

```bash
 [Info@Game] Is entity deleted successfully? 1
```

# Checking and detroying components

Just like entities, components can also be checked to see whether they exist and can be destroyed. Checking if a component exists is done with \ref Tourmaline::Systems::ECS::World::HasComponent, and the component can be removed with \ref Tourmaline::Systems::ECS::World::RemoveComponent.

```c++
#include <Tourmaline/Systems/ECS.hpp>

using namespace Tourmaline;
using namespace Tourmaline::Systems;

struct Stats : public ECS::Component {
  double HP = 100, maxHP = 100;
  double mana = 200, maxMana = 200;
};

int main() {
  ECS::World world;

  ECS::Entity entity = world.CreateEntity();
  ECS::Entity player =
      world.CreateEntity(false, Type::UUID("6B62E1F98D234BE92E6D93A3C7B0FDCA"));

  Components::Transform &playerTransform =
      world.GetComponent<Components::Transform>(player);

  playerTransform.Translate = {3, 0.5, -10.2};

  world.AddComponent<Stats>(player, true, 200, 200);
  Stats &entitysStats = world.AddComponent<Stats>(entity, true);

  entitysStats.isEnabled = false;
  world.SetEntityEnable(player, true);

  world.SetEntityLabel(player, "Player Character");
  auto entityName = world.GetEntityLabel(entity);
  auto playerName = world.GetEntityLabel(player);
  Logging::Log(entityName, "Game");
  Logging::Log(playerName, "Game");

  if (world.EntityExists(entity)) {
    bool isDeleted = world.DestroyEntity(entity);
    Logging::LogFormatted("Is entity deleted successfully? {}", "Game",
                          Logging::Info, isDeleted);
  }

  // Just like before with entity deletion, it is highly advised to verify that
  // a component exists before destroying it.
  if (world.HasComponent<Stats>(player)) {
    // You can also check if the component was successfully deleted using the
    // returned value, as with entities.
    // Tourmaline::Systems::Components::Transform cannot be destroyed!
    bool isDeleted = world.RemoveComponent<Stats>(player);
    Logging::LogFormatted(
        "Is player's Stats component deleted successfully? {}", "Game",
        Logging::Info, isDeleted);
  }

  return 0;
}
```

Output:

```bash
  [Info@Game] Is player's Stats component deleted successfully? 1
```

# Further reading

This example only focuses on the Entity and Component aspects of the ECS system. In the ECS Advanced example, we will be delving into how to add Systems and how to use Prefabs.

You can also get this entire example as a source file here: \subpage ECS-Basics-Example.

You can continue learning more about Tourmaline Engine at \ref examples.
