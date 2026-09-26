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
#include <tuple>
#include <type_traits>

/**
 * @file
 * @brief Serialization functions to define how to serialize/deserialize data
 */
namespace Tourmaline::Systems::Serialization {
template <typename Type> struct Serializable;

// Default
/**
 * @brief Template function used to convert a data type to a string for
 * serialization.
 *
 * @tparam type The type this function applies to. You can also use C++20
 * concepts to make a broader function.
 *
 * @return The data as a string for serialization.
 *
 * @warning The first 4 bytes must contain the length of the data being written.
 * So, if you are writing 20 bytes of data, the first 4 bytes as a uint32_t must 
 * have the value 20.
 * Failure to add the proper length will break the rest of the serialization.
 */
template <typename type> static std::string ToData(type) {
  static_assert(false, "ToData function requires a overload defined for this "
                       "type to serialize data.");
  throw;
}

/**
 * @brief Template function used to convert a serialized string back into
 * its data type during deserialization.
 *
 * @tparam type The type this function applies to. You can also use C++20
 * concepts to make a broader function.
 *
 * @param dataStart Where the data starts. The first 4 bytes will contain the
 * length of the data.
 * @param result Reference to where the result of the deserialization should be written.
 *
 * @warning The first 4 bytes contain how long the data inside dataStart
 * is, as a uint_32_t. So, if the data stored in dataStart is 20 bytes, the first 4 bytes,
 * as a uint32_t, will have the value 20.
 */
template <typename type>
static void FromData(const char *dataStart, type &result) {
  // To supress warns
  std::ignore = dataStart;
  std::ignore = result;
  static_assert(false, "FromData function requires a overload defined for this "
                       "type to deserialize data.");
  throw;
}

// Functions
/**
 * @brief Functions cannot be serialized/deserialized.
 */
template <typename type>
  requires std::is_function_v<type>
static std::string ToData(type) {
  static_assert(false, "Functions cannot be serialized!");
  throw;
}

/**
 * @brief Functions cannot be serialized/deserialized.
 */
template <typename type>
  requires std::is_function_v<type>
static void FromData(const char *, type &) {
  static_assert(false, "Functions cannot be deserialized!");
  throw;
}

// Pointers
/**
 * @brief Currently pointers and references are not supported for
 * serialization/deserialization.
 *
 * It is still possible to do so, if a per type of pointer/reference overload is defined for
 * the ToData/FromData functions.
 */
template <typename type>
  requires std::is_reference_v<type> || std::is_pointer_v<type>
static std::string ToData(type) {
  static_assert(
      false,
      "Pointers/References are currently not supported for serialization. "
      "This is a planned feature to be added.");
  throw;
}

/**
 * @brief Currently pointers and references are not supported for
 * serialization/deserialization.
 *
 * It is still possible to do so, if a per type of pointer/reference overload is defined for
 * the ToData/FromData functions.
 */
template <typename type>
  requires std::is_reference_v<type> || std::is_pointer_v<type>
static void FromData(const char *, type &) {
  static_assert(false, "Functions cannot be deserialized!");
  throw;
}

// Classes with Serialization support
/**
 * @brief Calls Serialize() recursively so a class inside a class can be
 * serialized as well. The inner class must also inherit Serializable.
 */
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

/**
 * @brief Calls Deserialize() recursively so a class inside a class can be
 * deserialized as well. The inner class must also inherit Serializable.
 */
template <typename type>
  requires std::is_base_of_v<Serializable<type>, type>
static void FromData(const char *entryLocation, type &result) {
  const uint32_t length = *reinterpret_cast<const uint32_t *>(entryLocation);
  std::string data(entryLocation + sizeof(uint32_t), length);
  result.Deserialize(data);
}

// Fundamental data types
/**
 * @brief Fundamental types will be serialized automatically.
 */
template <typename type>
  requires std::is_fundamental_v<type>
static std::string ToData(type value) {
  std::string result;
  std::uint32_t size = sizeof(type);
  result.append(reinterpret_cast<const char *>(&size), sizeof(decltype(size)));
  result.append(reinterpret_cast<const char *>(&value), sizeof(type));

  return result;
}

/**
 * @brief Fundamental types will be deserialized automatically.
 */
template <typename type>
  requires std::is_fundamental_v<type>
static void FromData(const char *entryLocation, type &result) {
  // Length is not necessary with fundamentals
  result =
      *const_cast<type *>(reinterpret_cast<const type *>(entryLocation + 4));
}
} // namespace Tourmaline::Systems::Serialization
#endif
