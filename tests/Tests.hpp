/*
 * SPDX-FileCopyrightText: Dora "cat" <cat@thenight.club>
 * SPDX-License-Identifier: MPL-2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at http://mozilla.org/MPL/2.0/.
 */
#ifndef GUARD_TOURMALINE_TESTS_H
#define GUARD_TOURMALINE_TESTS_H

#include "Corrade/Containers/String.h"
#include "Corrade/TestSuite/Tester.h"
#include "Systems/ECS.hpp"

#define CORRADE_VERIFY_TYPE(x, y) CORRADE_VERIFY(typeid(x) == typeid(y))

class Tests : public Corrade::TestSuite::Tester {
public:
  Tests() {
    // Random
    addTests({&Tests::Random_SetSeed});
    addBenchmarks({&Tests::Random_Benchmark_Generating1kIntegers},
                  randomBatchCount);

    // ECS
    addBenchmarks(
        {&Tests::ECS_Benchmark_GoalCreate, &Tests::ECS_Benchmark_FetchAndStep},
        1);
    addBenchmarks({&Tests::ECS_Benchmark_Step}, stepBatchCount);
    addBenchmarks({&Tests::ECS_Benchmark_60Steps}, multiStepBatchCount);

    // Logging
    Tourmaline::Systems::Logging::WithANSIColour = false; // Less headache
    addTests({&Tests::Logging_ConsoleAndFileSync, &Tests::Logging_FileSwitching,
              &Tests::Logging_ErrorAndCritical});
  }

  // Tourmaline::Systems::Random
  static constexpr uint32_t randomBatchCount = 10;
  void Random_SetSeed();
  void Random_Benchmark_Generating1kIntegers();

  // Tourmaline::Systems::ECS
  static constexpr uint32_t ecsGoal = 50'000, multiStepBatchCount = 5,
                            stepBatchCount = 20;
  static Tourmaline::Systems::ECS::World world;
  void ECS_Benchmark_GoalCreate();
  void ECS_Benchmark_FetchAndStep();
  void ECS_Benchmark_60Steps();
  void ECS_Benchmark_Step();

  // Tourmaline::Systems::Logging
  static Corrade::Containers::String fakeStdOutName;
  void Logging_ConsoleAndFileSync();
  void Logging_FileSwitching();
  void Logging_ErrorAndCritical();

  // Helpers
  void Helper_SwitchSTDOUT(bool fakeSTD);
};

#endif
