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

#include "Corrade/TestSuite/Tester.h"

#define CORRADE_COMPARE_TYPE(x, y) CORRADE_COMPARE(typeid(x), typeid(y))
#define CORRADE_VERIFY_TYPE(x, y) CORRADE_VERIFY(typeid(x) == typeid(y))

class Tests : public Corrade::TestSuite::Tester {
public:
  static constexpr uint32_t batchCount = 10;
  Tests() {
    addTests({&Tests::Random_SetSeed, &Tests::Random_TypeCorretness});
    addBenchmarks({&Tests::Random_Benchmark_Generating1kIntegers}, batchCount);
  }

  // Tourmaline::Systems::Random
  void Random_SetSeed();
  void Random_TypeCorretness();
  void Random_Benchmark_Generating1kIntegers();
};

#endif
