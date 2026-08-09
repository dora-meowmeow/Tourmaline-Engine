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
#include "../Types.hpp"
#include "TourmalineExternal/random/xoshiro.h"

#include <type_traits>
/**
 * @file
 * @brief Built-in random generation
 */
namespace Tourmaline::Systems {
/**
 * @brief Static random generation class. The random generation uses xoshiro.h.
 * @see https://github.com/david-cortes/xoshiro_cpp/blob/master/xoshiro.h
 */
class Random {
public:
  /**
   * @brief Generates a random value.
   * @tparam T any type that satisfies std::is_integral_v
   * @param max maximum inclusive value to generate.
   * @param min minimum inclusive value to generate.
   * @return a random value same type as max and min.
   */
  template <typename T>
    requires std::is_integral_v<T>
  static T Generate(T max, T min = 0) {
    return (generator() % (max - min + 1)) + min;
  }

  /**
   * @brief Generates a random Tourmaline::Type::UUID.
   * @return a random Tourmaline::Type::UUID.
   */
  static Tourmaline::Type::UUID GenerateUUID();

private:
  static Xoshiro::Xoshiro256PP generator;
};
} // namespace Tourmaline::Systems
#endif
