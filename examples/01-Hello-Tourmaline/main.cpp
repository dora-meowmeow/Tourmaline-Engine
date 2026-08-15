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
#include <Tourmaline/Systems/Logging.hpp>
#include <Tourmaline/Systems/Random.hpp>

using namespace Tourmaline;
using namespace Systems;

void move(const ECS::Entity &, Components::Transform &transform) {
  auto &translate = transform.Translate;
  translate.x() += Random::Generate(10) - 5;
  translate.y() += Random::Generate(10) - 5;
  translate.z() += Random::Generate(10) - 5;
}

void rotate(const ECS::Entity &, Components::Transform &transform) {
  auto &rotate = transform.Rotate;
  rotate.x() += 1;
}

void printLocation(const ECS::Entity &entity,
                   Components::Transform &transform) {
  auto &translate = transform.Translate;
  auto &rotate = transform.Rotate;

  Logging::LogFormatted(
      "Entity {} is at location ({}, {}, {}) with X rotation ({})", "Test",
      Logging::Info, entity.asString(), translate.x(), translate.y(),
      translate.z(), rotate.x());
}

constexpr uint8_t goal = 5;
constexpr uint8_t steps = 3;
int main() {
  ECS::World world{};

  for (uint8_t x = 0; x < goal; x++) {
    world.CreateEntity();
  }

  world.AddSystem(&move);
  world.AddSystem(&rotate, ECS::SystemPriority::Pre);
  world.AddSystem(&printLocation, ECS::SystemPriority::Post);

  for (uint8_t x = 0; x < steps; x++) {
    world.Step();
    Logging::Log("----- Next Step -----", "Test");
  }

  Logging::Log("Tourmaline ran the demo successfully", "Test");
}
