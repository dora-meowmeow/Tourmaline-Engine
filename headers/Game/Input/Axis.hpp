/*
 * SPDX-FileCopyrightText: Dora "cat" <cat@thenight.club>
 * SPDX-License-Identifier: MPL-2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef GUARD_TOURMALINE_AXIS_H
#define GUARD_TOURMALINE_AXIS_H

/**
 * @file
 * @brief Input axising
 */
#include "Keyboard.hpp"
#include "Magnum/Math/Vector2.h"

#include <type_traits>

namespace Tourmaline::Game::Input {
/**
 * @brief Quality of life key axis function.
 *
 * @tparam returnType The type to return as. Must be signed and either an
 * integer or a float type.
 *
 * @param positiveKey Key to be the positive value on the axis.
 * @param negativeKey Key to be the negative value on the axis.
 * @param stateToCompare Which state should the keys be compared. Default is
 * KeyState::Held.
 * @param scale Scales the result to specificed value.
 *
 * @return If only positiveKey's state equals to stateToCompare, returns
 * `scale`. If both keys' states equal to stateToCompare, returns `0`. If only
 * negativeKey's state equals to stateToCompare, returns `-scale`.
 */
template <typename returnType>
  requires(std::is_integral_v<returnType> ||
           std::is_floating_point_v<returnType>) &&
          std::is_signed_v<returnType>
returnType KeyAxis(const Input::Key &positiveKey, const Input::Key &negativeKey,
                   Input::KeyState stateToCompare = Input::KeyState::Held,
                   returnType scale = 1) {
  return static_cast<returnType>((positiveKey.state == stateToCompare) -
                                 (negativeKey.state == stateToCompare)) *
         scale;
}

/**
 * @brief Vector2D variant of Tourmaline::Game::Input::KeyAxis.
 *
 * @tparam returnType The type to return as. Must be signed and either an
 * integer or a float type.
 *
 * @param xPositiveKey Key to be the positive value on the X axis.
 * @param xNegativeKey Key to be the negative value on the X axis.
 * @param yPositiveKey Key to be the positive value on the Y axis.
 * @param yNegativeKey Key to be the negative value on the Y axis.
 * @param stateToCompare Which state should the keys be compared. Default is
 * KeyState::Held.
 * @param scale Scales the result to specificed value.
 *
 * @return Vector2D axis multiplied scale.
 * __**If returnType is a floating point**__, vector2D is normalised then
 * multiplied with scale.
 */
template <typename returnType>
  requires(std::is_integral_v<returnType> ||
           std::is_floating_point_v<returnType>) &&
          std::is_signed_v<returnType>
Magnum::Math::Vector2<returnType>
KeyAxis2D(const Input::Key &xPositiveKey, const Input::Key &xNegativeKey,
          const Input::Key &yPositiveKey, const Input::Key &yNegativeKey,
          Input::KeyState stateToCompare = Input::KeyState::Held,
          returnType scale = 1) {
  Magnum::Math::Vector2<returnType> result{
      KeyAxis(xPositiveKey, xNegativeKey, stateToCompare, 1),
      KeyAxis(yPositiveKey, yNegativeKey, stateToCompare, 1)};
  if constexpr (std::is_floating_point_v<returnType>) {
    return result.length() ? result.normalized() * scale : result;
  }
  return result * scale;
}

} // namespace Tourmaline::Game::Input
#endif
