/*
 * SPDX-FileCopyrightText: Dora "cat" <cat@thenight.club>
 * SPDX-License-Identifier: MPL-2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at http://mozilla.org/MPL/2.0/.
 */
#include "../Random.hpp"
#include <bit>
#include <cstdint>
#include <ctime>

using namespace Tourmaline::Systems;

Xoshiro::Xoshiro256PP Random::generator(static_cast<uint64_t>(time(NULL)));
Tourmaline::Type::UUID Random::GenerateUUID() {
  uint64_t random_ab = generator(), random_c = generator(), hold = 0;
  hold = std::rotr(random_ab, 12);
  hold = hold >> 4;
  hold = (hold << 4) + 0b0100;
  random_ab = std::rotl(hold, 12);
  random_c = ((random_c >> 2) << 2) + 2;

  return Tourmaline::Type::UUID(random_ab, random_c);
}
