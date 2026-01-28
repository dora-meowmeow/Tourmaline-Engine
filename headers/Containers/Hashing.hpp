/*
 * SPDX-FileCopyrightText: Dora "cat" <cat@thenight.club>
 * SPDX-License-Identifier: MPL-2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef GUARD_TOURMALINE_HASHING_H
#define GUARD_TOURMALINE_HASHING_H
#include <concepts>
#include <functional>

namespace Tourmaline::Containers {
template <typename T>
concept Hashable = std::equality_comparable<T> && requires(T x) {
  { std::hash<T>{}(x) } -> std::convertible_to<std::size_t>;
};
} // namespace Tourmaline::Containers
#endif
