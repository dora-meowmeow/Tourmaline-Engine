/*
 * SPDX-FileCopyrightText: Dora "cat" <cat@thenight.club>
 * SPDX-License-Identifier: MPL-2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "Systems/Random.hpp"
#include "Corrade/TestSuite/Tester.h"
#include "Tests.hpp"

#include <array>
#include <cstdint>

using namespace Tourmaline::Systems;

void Tests::Random_SetSeed() {
  uint64_t originalSeed = Random::GetSeed();
  std::array<int32_t, 10> expectedValues{
      517549132, 554690863,   -326009147, 935928041,  325326953,
      906887419, -1433976211, 927017409,  -136274294, 644544249};
  Random::SetSeed(20250320);

  for (uint64_t x = 0; x < 10; x++) {
    CORRADE_COMPARE(Random::Generate(INT32_MAX - 1, INT32_MIN + 1),
                    expectedValues[x]);
  }
  Random::SetSeed(originalSeed);
}

void Tests::Random_Benchmark_Generating1kIntegers() {
  CORRADE_BENCHMARK(randomBatchCount) {
    for (uint32_t x = 0; x < 1000; x++) {
      Tourmaline::Systems::Random::Generate(INT32_MAX - 1, INT32_MIN + 1);
    }
  }
}
