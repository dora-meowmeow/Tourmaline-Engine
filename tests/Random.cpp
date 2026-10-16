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

#include <cstdint>

using namespace Tourmaline::Systems;

void Tests::Random_SetSeed() {
  static int32_t expectedValues[10]{
      517549132, 554690863,   -326009147, 935928041,  325326953,
      906887419, -1433976211, 927017409,  -136274294, 644544249};
  uint64_t originalSeed = Random::GetSeed();
  Random::SetSeed(20250320);
  for (uint64_t x = 0; x < 10; x++) {
    CORRADE_COMPARE(Random::Generate(INT32_MAX - 1, INT32_MIN + 1),
                    expectedValues[x]);
  }
  Random::SetSeed(originalSeed);
}

void Tests::Random_TypeCorretness() {
  // Making sure the generation respects the type
  CORRADE_VERIFY_TYPE(Random::Generate(64l), long);
  CORRADE_VERIFY_TYPE(Random::Generate(32), int);
  CORRADE_VERIFY_TYPE(Random::Generate(static_cast<short>(16)), short);
  CORRADE_VERIFY_TYPE(Random::Generate(static_cast<char>(8)), char);
  CORRADE_VERIFY_TYPE(Random::Generate(static_cast<int8_t>(4)), int8_t);
  CORRADE_VERIFY_TYPE(Random::Generate(static_cast<uint8_t>(2)), uint8_t);
  CORRADE_VERIFY_TYPE(Random::Generate(static_cast<bool>(1)), bool);
  CORRADE_VERIFY_TYPE(Random::Generate(0.5f), float);
  CORRADE_VERIFY_TYPE(Random::Generate(0.25), double);
  // The wriiiisssttt gaaaaammmmmee1!!!111!!!!
}

void Tests::Random_Benchmark_Generating1kIntegers() {
  CORRADE_BENCHMARK(batchCount) {
    for (uint32_t x = 0; x < 1000; x++) {
      Tourmaline::Systems::Random::Generate(INT32_MAX - 1, INT32_MIN + 1);
    }
  }
}
