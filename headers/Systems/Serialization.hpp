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
#include <string>
#include <tuple>
#include <utility>

#include "Serialization/DataHandling.hpp"

namespace Tourmaline::Systems::Serialization {
template <typename Type> struct Serializable {
  virtual ~Serializable() = default;

  template <typename... dataType>
  struct SerialList : public std::tuple<dataType Type::*...> {
    SerialList(dataType Type::*...memberPointers)
        : std::tuple<dataType Type::*...>(memberPointers...) {}
    static constexpr std::size_t Count = sizeof...(dataType);
  };

  std::string Serialize() {
    const Type &instance = getTypeInstance();
    auto serialList = getSerialList();
    return [&]<std::size_t... index>(std::index_sequence<index...>) {
      return insertMetadata(serialList.Count) +
             ((ToData(instance.*(std::get<index>(serialList)))) + ...);
    }(std::make_integer_sequence<std::size_t, serialList.Count>{});
  }

  void Deserialize(std::string serialData) {
    Type &instance = getTypeInstance();
    auto serialList = getSerialList();
    char *data = serialData.data();
    uint8_t serialEndianess = data[0];

    if (serialEndianess == (std::endian::native == std::endian::little))
        [[likely]] {
      const uint32_t count = *reinterpret_cast<const uint32_t *>(data + 1);
      char *dataPointers[count]; // This is char* since data length is unit byte
      dataPointers[0] = data + sizeof(uint32_t) + 1;
      for (uint32_t x = 1; x < count; x++) {
        uint32_t length = *reinterpret_cast<uint32_t *>(dataPointers[x - 1]);
        dataPointers[x] = dataPointers[x - 1] + sizeof(uint32_t) + length;
      }
      [&]<std::size_t... index>(std::index_sequence<index...>) {
        ((FromData(dataPointers[index],
                   instance.*(std::get<index>(serialList)))),
         ...);
      }(std::make_integer_sequence<std::size_t, serialList.Count>{});
    }
  }

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
