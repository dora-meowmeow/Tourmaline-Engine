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
#include <tuple>
#include <type_traits>

/**
 * @file
 * @brief Concepts and template meta programming.
 */

namespace Tourmaline::Concepts {
/**
 * @brief A type that has a hash function and is equality comparable.
 * @tparam T Type to check.
 */
template <typename T>
concept Hashable = std::equality_comparable<T> && requires(T x) {
  { std::hash<T>{}(x) } -> std::convertible_to<std::size_t>;
};

/**
 * @brief A type that is constrained to be either the same as Type1 or Type2.
 * @tparam Base Type to check.
 * @tparam Type1 First type to check against.
 * @tparam Type2 Second type to check against.
 */
template <typename Base, typename Type1, typename Type2>
concept Either = std::same_as<Base, Type1> || std::same_as<Base, Type2>;

// Oh C++ and your jank
/// @cond
template <typename Base, typename Type1, typename Type2> struct _opposite_of {
  using type = std::conditional_t<std::is_same_v<Base, Type1>, Type2, Type1>;
};
/// @endcond

/**
 * @brief Returns whichever type Base does not equal, between Type1 and Type2. 
 * @tparam Base Type to check.
 * @tparam Type1 First type. If Base is same as Type1, then Type2 will be
 * returned.
 * @tparam Type2 Second type. If Base is same as Type2, then Type1 will be
 * returned.
 */
template <typename Base, typename Type1, typename Type2>
  requires Either<Base, Type1, Type2>
using OppositeOf = _opposite_of<Base, Type1, Type2>::type;

/// @cond
template <typename> struct FunctionTraits;
template <typename Function>
struct FunctionTraits
    : public FunctionTraits<
          decltype(&std::remove_reference_t<Function>::operator())> {};

template <typename Return, typename Class, typename... Arguments>
struct FunctionTraits<Return (Class::*)(Arguments...) const>
    : FunctionTraits<Return (*)(Arguments...)> {};
template <typename Return, typename Class, typename... Arguments>
struct FunctionTraits<Return (Class::*)(Arguments...)>
    : FunctionTraits<Return (*)(Arguments...)> {};
/// @endcond

/**
 * @brief Contains information regarding how a function is structured.
 * @note heavily inspired by sql_modern_cpp's implementation
 * https://github.com/aminroosta/sqlite_modern_cpp/blob/master/hdr/sqlite_modern_cpp/utility/function_traits.h
 */
template <typename Return, typename... Arguments>
struct FunctionTraits<Return (*)(Arguments...)> {
  /// @brief The return type of the function.
  using returnType = Return;
  /// @brief Arguments the function has, stored in a tuple.
  using arguments = std::tuple<Arguments...>;

  /// @brief Fetches an argument by using its index.
  template <std::size_t index>
  using argument = std::tuple_element_t<index, arguments>;

  /// @brief Amount of arguments the function has.
  static constexpr std::size_t argumentCount = sizeof...(Arguments);
};
} // namespace Tourmaline::Concepts
#endif
