/*
 * SPDX-FileCopyrightText: Dora "cat" <cat@thenight.club>
 * SPDX-License-Identifier: MPL-2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef GUARD_TOURMALINE_SERIALIZATION_H
#define GUARD_TOURMALINE_SERIALIZATION_H
#include <bit>
#include <cstdint>
#include <memory>
#include <string>
#include <string_view>
#include <tuple>
#include <utility>

#include "Serialization/DataHandling.hpp"

/**
 * @file
 * @brief Built-in serialization feature
 */

namespace Tourmaline::Systems::Serialization {
/**
 * @brief When inherited publically, it allows serialization of a class.
 *
 * @tparam Type Type of the class that is inheriting from (CRTP).
 *
 * Marks a class/struct as serializable. This allows Tourmaline to serialize,
 * and deserialize any member with given pointer inside `serialList` (see note).
 *
 * @note Every class that inherits Serializable must declare a
 * `static inline SerialList serialList{};`. The name MUST be `serialList`.
 * see @ref Tourmaline::Systems::Serialization::Serializable::SerialList to
 * learn more.
 *
 * @warning As stated in the note before it must be static, also the type and
 * the name must be EXACT. You cannot name it anything else or use a different
 * type or make it not static. It may not be inline, if you prefer.
 */
template <typename Type> struct Serializable {
  /**
   * @brief Member pointer list(tuple) of every member that should be
   * serialized.
   *
   * @tparam dataType The data type of each member pointer. Should resolve
   * automatically.
   *
   * This struct will hold every member point that is requested to be
   * serialized/deserialized. This does allow you to skip some members of a
   * class from serialization/deserialization. Any member that is not
   * serialized/deserialized, will not be affected/changed.
   *
   * @note As stated in @ref Tourmaline::Systems::Serialization::Serializable
   * Each class/struct that inherits Serializable requires to define `static
   * inline SerialList serialList{};`. Everything except the `inline` is
   * required. The name MUST be `serialList` and nothing else.
   */
  template <typename... dataType>
  struct SerialList : public std::tuple<dataType Type::*...> {
    /**
     * @brief Constructs the list with given member pointers.
     *
     * @param memberPointers Every member that should be serialized's member
     * pointer (example: &x::y).
     *
     * Under the hood this just generates an std::tuple. However it deals with a
     * lot of type specification requirements.
     */
    SerialList(dataType Type::*...memberPointers)
        : std::tuple<dataType Type::*...>(memberPointers...) {}
    /**
     * @brief Amount of members to serialize/deserialize.
     */
    static constexpr std::size_t Count = sizeof...(dataType);
  };

  virtual ~Serializable() = default;

  /**
   * @brief Serializes the class/struct members in serialList into a string.
   *
   * For each type a ToData and FromData must be defined for proper
   * serialization/deserialization. See
   * @ref Tourmaline::Systems::Serialization::ToData and
   * @ref Tourmaline::Systems::Serialization::FromData.
   *
   * @return The serialized data of the class.
   */
  std::string Serialize() {
    const Type &instance = getTypeInstance();
    auto serialList = getSerialList();
    return [&]<std::size_t... index>(std::index_sequence<index...>) {
      return insertMetadata(serialList.Count) +
             ((ToData(instance.*(std::get<index>(serialList)))) + ...);
    }(std::make_integer_sequence<std::size_t, serialList.Count>{});
  }

  /**
   * @brief Deserializes the string into the class/struct's members in
   * serialList.
   *
   * @param serialData The raw serialized data from Serialize() function.
   *
   * For each type a ToData and FromData must be defined for proper
   * serialization/deserialization. See
   * @ref Tourmaline::Systems::Serialization::ToData and
   * @ref Tourmaline::Systems::Serialization::FromData.
   */
  void Deserialize(std::string_view serialData) {
    Type &instance = getTypeInstance();
    auto serialList = getSerialList();
    const char *data = serialData.data();
    uint8_t serialEndianess = serialData[0];

    if (serialEndianess == (std::endian::native == std::endian::little))
        [[likely]] {
      const uint32_t count = *reinterpret_cast<const uint32_t *>(data + 1);

      // Doing this to not rely on VLA
      auto dataStore = std::make_unique<const char *[]>(count);

      // This is a char since data length is unit byte
      const char **dataPointers = dataStore.get();
      dataPointers[0] = data + sizeof(uint32_t) + 1;
      for (uint32_t x = 1; x < count; x++) {
        const uint32_t length =
            *reinterpret_cast<const uint32_t *>(dataPointers[x - 1]);
        dataPointers[x] = dataPointers[x - 1] + sizeof(uint32_t) + length;
      }
      [&]<std::size_t... index>(std::index_sequence<index...>) {
        ((FromData(dataPointers[index],
                   instance.*(std::get<index>(serialList)))),
         ...);
      }(std::make_integer_sequence<std::size_t, serialList.Count>{});
    }
  }

  /**
   * @brief Returns the class instance that inherits this class (CRTP).
   *
   * @return The class instance as a reference.
   */
  Type &getTypeInstance() const {
    return *const_cast<Type *>(reinterpret_cast<const Type *>(this));
  };

private:
  std::string insertMetadata(std::uint32_t entryCount) {
    std::string result{1}; // Little endian assumed by default
    if constexpr (std::endian::native == std::endian::big) {
      result[0] = 0;
    }
    result.append(reinterpret_cast<const char *>(&entryCount),
                  sizeof(decltype(entryCount)));
    return result;
  }
  auto getSerialList() const { return Type::serialList; }
};
} // namespace Tourmaline::Systems::Serialization

#endif
