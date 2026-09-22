/*
 * SPDX-FileCopyrightText: Dora "cat" <cat@thenight.club>
 * SPDX-License-Identifier: MPL-2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef GUARD_TOURMALINE_SERIAL_DATAHANDLING_H
#define GUARD_TOURMALINE_SERIAL_DATAHANDLING_H
#include <cstdint>
#include <string>
#include <type_traits>

namespace Tourmaline::Systems::Serialization {
template <typename Type> struct Serializable;

// Default
template <typename type> static std::string ToData(type) {
  static_assert(false, "ToData function requires a overload defined for this "
                       "type to serialize data.");
  throw;
}

template <typename type> static void FromData(char *, type &) {
  static_assert(false, "FromData function requires a overload defined for this "
                       "type to deserialize data.");
  throw;
}

// Functions
template <typename type>
  requires std::is_function_v<type>
static std::string ToData(type) {
  static_assert(false, "Functions cannot be serialized!");
  throw;
}

template <typename type>
  requires std::is_function_v<type>
static void FromData(char *, type &) {
  static_assert(false, "Functions cannot be deserialized!");
  throw;
}

// Pointers
template <typename type>
  requires std::is_reference_v<type> || std::is_pointer_v<type>
static std::string ToData(type) {
  static_assert(
      false,
      "Pointers/References are currently not supported for serialization. "
      "This is a planned feature to be added.");
  throw;
}

// Classes with Serialization support
template <typename type>
  requires std::is_base_of_v<Serializable<type>, type>
static std::string ToData(type value) {
  std::string result;
  std::string serialized = value.Serialize();
  std::uint32_t size = serialized.length();
  result.append(reinterpret_cast<const char *>(&size), sizeof(decltype(size)));
  result += serialized;
  return result;
}

template <typename type>
  requires std::is_base_of_v<Serializable<type>, type>
static void FromData(char *entryLocation, type &result) {
  uint32_t length = *reinterpret_cast<uint32_t *>(entryLocation);
  std::string data(entryLocation + sizeof(uint32_t), length);
  result.Deserialize(data);
}

// Fundamental data types
template <typename type>
  requires std::is_fundamental_v<type>
static std::string ToData(type value) {
  std::string result;
  std::uint32_t size = sizeof(type);
  result.append(reinterpret_cast<const char *>(&size), sizeof(decltype(size)));
  result.append(reinterpret_cast<const char *>(&value), sizeof(type));

  return result;
}

template <typename type>
  requires std::is_fundamental_v<type>
static void FromData(char *entryLocation, type &result) {
  // Length is not necessary with fundamentals
  result = *reinterpret_cast<type *>(entryLocation + 4);
}
} // namespace Tourmaline::Systems::Serialization
#endif
