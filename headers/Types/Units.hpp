/*
 * SPDX-FileCopyrightText: Dora "cat" <cat@thenight.club>
 * SPDX-License-Identifier: MPL-2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef GUARD_TOURMALINE_TYPE_UNITS_H
#define GUARD_TOURMALINE_TYPE_UNITS_H
/**
 * @file
 * @brief Unit types definitions
 */
namespace Tourmaline::Type {
/**
 * @brief Which type of angles you want to use.
 *
 * This is mostly used to ask whether to use Magnum::Math::Rad Or
 * Magnum::Math::Deg inside a function.
 */
enum class AngleUnit { Radians = 0, Degrees = 1 };

/**
 * @brief Whether something should be applied local or globally.
 *
 * Usually used by matrix functions to decide; if the matrix being multiplied should
 * be on the right (this is known as local), or left (this is known as global).
 *
 * Most of the time local (putting the pure transformation to the right) is the
 * desired outcome. However if you want to affect something from the perspective of
 * the world, try using global.
 *
 * @see [3blue1brown's introduction on matrix multiplication and
 * order](https://www.3blue1brown.com/lessons/matrix-multiplication/).
 */
enum class Apply { Globally = 0, Locally = 1 };

} // namespace Tourmaline::Type

#endif
