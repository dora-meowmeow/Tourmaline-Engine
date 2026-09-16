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
returnType KeyAxis1D(const Input::Key &positiveKey,
                     const Input::Key &negativeKey,
                     Input::KeyState stateToCompare = Input::KeyState::Held,
                     returnType scale = 1) {
  return static_cast<returnType>((positiveKey.state == stateToCompare) -
                                 (negativeKey.state == stateToCompare)) *
         scale;
}

} // namespace Tourmaline::Game::Input
#endif
