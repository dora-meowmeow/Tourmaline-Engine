/*
 * SPDX-FileCopyrightText: Dora "cat" <cat@thenight.club>
 * SPDX-License-Identifier: MPL-2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef GUARD_TOURMALINE_RANDOM_H
#define GUARD_TOURMALINE_RANDOM_H
#include "../../libraries/random/xoshiro.h"
#include "../Types.hpp"
#include <type_traits>

namespace Tourmaline::Systems {
class Random {
public:
  template <typename T>
    requires std::is_integral_v<T>
  static T Generate(T max, T min = 0) {
    return (generator() % (max - min + 1)) + min;
  }
  static Tourmaline::Type::UUID GenerateUUID();

private:
  static Xoshiro::Xoshiro256PP generator;
};
} // namespace Tourmaline::Systems
#endif
