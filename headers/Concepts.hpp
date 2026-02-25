/*
 * SPDX-FileCopyrightText: Dora "cat" <cat@thenight.club>
 * SPDX-License-Identifier: MPL-2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef GUARD_TOURMALINE_CONCEPTS_H
#define GUARD_TOURMALINE_CONCEPTS_H
#include <concepts>
#include <functional>

namespace Tourmaline::Concepts {
template <typename T>
concept Hashable = std::equality_comparable<T> && requires(T x) {
  { std::hash<T>{}(x) } -> std::convertible_to<std::size_t>;
};

template <typename Base, typename Type1, typename Type2>
concept Either = std::same_as<Base, Type1> || std::same_as<Base, Type2>;

// Oh C++ and your jank
template <typename Base, typename Type1, typename Type2> struct _opposite_of {
  using type = std::conditional_t<std::is_same_v<Base, Type1>, Type2, Type1>;
};

template <typename Base, typename Type1, typename Type2>
  requires Either<Base, Type1, Type2>
using OppositeOf = _opposite_of<Base, Type1, Type2>::type;

} // namespace Tourmaline::Concepts
#endif
