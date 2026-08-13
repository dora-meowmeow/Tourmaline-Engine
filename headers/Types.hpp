/*
 * SPDX-FileCopyrightText: Dora "cat" <cat@thenight.club>
 * SPDX-License-Identifier: MPL-2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef GUARD_TOURMALINE_TYPES_H
#define GUARD_TOURMALINE_TYPES_H

#include "Corrade/Containers/String.h"
#include "TourmalineExternal/random/xoshiro.h"

#include <cstdint>
#include <functional>
#include <string>
/**
 * @file
 * @brief Custom types used by Tourmaline.
 */
namespace Tourmaline::Type {
/**
 * @brief A 128-bit UUID with hashing support.
 * @see https://www.rfc-editor.org/rfc/rfc9562.html#name-uuid-version-4
 */
class UUID {
public:
  [[nodiscard]]
  /**
   * @brief Outputs the UUID as a string.
   * @return returns a Corrade::Containers::String of the UUID.
   */
  Corrade::Containers::String asString() const;

  /// @cond
  bool operator==(const UUID &rhs) const;
  /// @endcond

  /**
   * @brief Creates an UUID with 2 specified 64-bit unsigned integers.
   * @see https://www.rfc-editor.org/rfc/rfc9562.html#name-uuid-version-4
   * @param firstHalf a 64-bit unsigned integer. This is 0 to 63rd bit of the
   * UUID.
   * @param secondHalf a 64-bit unsigned integer. This is 64th to 127th bit of
   * the UUID.
   * @note It is suggested to use Tourmaline::Systems::Random::GenerateUUID() to
   * generate new UUIDs.
   */
  UUID(uint64_t firstHalf = 0, uint64_t secondHalf = 0);
  /**
   * @brief Create an UUID with a string.
   * @param uuid Must be a hex representation with no dividers.
   * Example: "6B62E1F98D234BE92E6D93A3C7B0FDCA" is a valid UUID
   * @note It is suggested to use Tourmaline::Systems::Random::GenerateUUID() to
   * generate new UUIDs.
   * @warning This function does not check if the given UUID string is valid!
   */
  UUID(const std::string &uuid);

  /// @brief The first half of the UUID as 64 bit unsigned integer.
  uint64_t firstHalf;

  /// @brief The first half of the UUID as 64 bit unsigned integer.
  uint64_t secondHalf;
};

/**
 * @brief A placeholder struct for templates. If a template has a type
 * set as this, that type (and possibly the argument itself) is optional.
 */
struct UnspecifiedType {};
} // namespace Tourmaline::Type

/// @cond
namespace std {
template <> struct hash<Tourmaline::Type::UUID> {
  size_t operator()(const Tourmaline::Type::UUID &uuid) const noexcept {
    uint64_t hash = Xoshiro::splitmix64(uuid.firstHalf);
    hash += uuid.secondHalf;
    return Xoshiro::splitmix64(hash);
  }
};

} // namespace std
/// @endcond
#endif
