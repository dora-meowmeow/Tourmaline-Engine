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
#include "Systems/Random.hpp"

#include "Corrade/TestSuite/Tester.h"
#include "Tests.hpp"

using namespace Tourmaline;
using namespace Systems;

Tourmaline::Systems::ECS::World Tests::world{ecsGoal};

void moveWithRandom(const ECS::Entity &, Components::Transform &transform) {
  transform.Matrix.translate({Random::Generate(10.f) - 5,
                              Random::Generate(10.f) - 5,
                              Random::Generate(10.f) - 5});
}

void move(const ECS::Entity &, Components::Transform &transform) {
  transform.Matrix.translate({1.0f, -2.0f, 5.0f});
}

void rotate(const ECS::Entity &, Components::Transform &transform) {
  transform.Matrix.rotateEuler({1.0f, 0.0f, 0.0f});
}

void Tests::ECS_Benchmark_GoalCreate() {
  CORRADE_BENCHMARK(1) {
    for (uint64_t x = 0; x < ecsGoal; x++) {
      ECS::Entity entity = world.CreateEntity();
      // Random is slower
      if (x % 100 == 0) [[unlikely]] {
        world.SetEntityEnable(entity, false);
      }
    }
  }

  world.AddSystem(&moveWithRandom);
  world.AddSystem(&rotate, ECS::SystemPriority::Pre);
}

void Tests::ECS_Benchmark_FetchAndStep() {
  CORRADE_BENCHMARK(1) { world.Step(); }
}

void Tests::ECS_Benchmark_60Steps() {
  CORRADE_BENCHMARK(multiStepBatchCount) {
    uint16_t t = 61;
    while (t-- != 0) {
      world.Step();
    }
  }
}

void Tests::ECS_Benchmark_Step() {
  CORRADE_BENCHMARK(stepBatchCount) { world.Step(); }
}
