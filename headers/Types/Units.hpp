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
 * @brief Unit types definition
 */
namespace Tourmaline::Type {
/**
 * @brief Which type of angles you want to use.
 *
 * This is mostly used to ask wheter to use Magnum::Math::Rad Or
 * Magnum::Math::Deg inside a function.
 */
enum class AngleUnit { Radiants = 0, Degrees = 1 };

/**
 * @brief Wheter or not something should be applied local or globally.
 *
 * Usually used by matrix functions to decide if the matrix should be
 * on the left() or right of the other matrix.
 */
enum class Apply { Globally = 0, Locally = 1 };

} // namespace Tourmaline::Type

#endif
