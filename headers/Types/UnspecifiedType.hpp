/*
 * SPDX-FileCopyrightText: Dora "cat" <cat@thenight.club>
 * SPDX-License-Identifier: MPL-2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef GUARD_TOURMALINE_TYPE_UNSPECIFIED_H
#define GUARD_TOURMALINE_TYPE_UNSPECIFIED_H
/**
 * @file
 * @brief UnspecifiedType type definition
 */
namespace Tourmaline::Type {
/**
 * @brief A placeholder struct for templates. If a template has a type
 * set as this, that type (and possibly the argument itself) is optional.
 */
struct UnspecifiedType {};
} // namespace Tourmaline::Type

#endif
