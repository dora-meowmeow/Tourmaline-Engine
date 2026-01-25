/*
 * SPDX-FileCopyrightText: Dora "cat" <cat@thenight.club>
 * SPDX-License-Identifier: MPL-2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at http://mozilla.org/MPL/2.0/.
 */
#ifndef GUARD_TOURMALINE_DUALKEYMAP_H
#define GUARD_TOURMALINE_DUALKEYMAP_H
#include "../Systems/Logging.hpp"
#include "Hashing.hpp"

#include <cstddef>
#include <cstdint>
#include <functional>
#include <optional>
#include <span>
#include <utility>
#include <variant>
#include <vector>

namespace Tourmaline::Containers {
template <Hashable AKey, Hashable BKey, typename Value,
          uint64_t baseReservation = 1024,
          float reservationGrowthExponent = 1.5>
class DualkeyMap {
  DualkeyMap() { HashList.reserve(baseReservation); }

  ~DualkeyMap() {
    // I'm sure there is a better way to do this
    for (DualkeyHash hash : HashList) {
      delete hash.Apointer;
      delete hash.Bpointer;
      delete hash.ValuePointer;
    }
  }

  std::span<std::pair<std::variant<std::monostate, AKey &, BKey &>, Value &>>
  operator[](std::optional<AKey> FirstKey, std::optional<BKey> SecondKey) {
    bool isFirstKeyGiven = FirstKey.has_value();
    bool isSecondKeyGiven = SecondKey.has_value();

    if (!isFirstKeyGiven && !isSecondKeyGiven) [[unlikely]] {
      Systems::Logging::Log("Failed to index! Dualkey maps require at least 1 "
                            "key to be given, returning an empty span.",
                            "Dualkey Map", Systems::Logging::LogLevel::Warning);
      return {};
    }
    std::size_t firstKeyHash =
        isFirstKeyGiven ? std::hash<AKey>{}(*FirstKey.value()) : 0;
    std::size_t secondKeyHash =
        isSecondKeyGiven ? std::hash<BKey>{}(*SecondKey.value()) : 0;

    std::vector<
        std::pair<std::variant<std::monostate, AKey &, BKey &>, Value &>>
        finishedQuery{};

    uint8_t stateOfIndexing = isFirstKeyGiven + (isSecondKeyGiven << 1);
    for (DualkeyHash hash : HashList) {
      switch (stateOfIndexing) {
      case 1: // Only first key is given
        if (firstKeyHash == hash.AKeyHash) {
          finishedQuery.emplace_back(hash.BPointer, hash.ValuePointer);
        }
        continue;
      case 2: // Only second key is given
        if (secondKeyHash == hash.BKeyHash) {
          finishedQuery.emplace_back(hash.APointer, hash.ValuePointer);
        }
        continue;
      case 3: // Both are given
        if (firstKeyHash == hash.AKeyHash && secondKeyHash == hash.BKeyHash) {
          finishedQuery.emplace_back(std::monostate{}, hash.ValuePointer);
        }
        break;
      }
      break;
    }

    return finishedQuery;
  }

  // No copying, No moving. Moving may be valid in the future.
  // However as of now it is not a wise way to use this map.
  DualkeyMap(const DualkeyMap &) = delete;
  DualkeyMap(DualkeyMap &&) = delete;
  DualkeyMap &operator=(const DualkeyMap &) = delete;
  DualkeyMap &operator=(DualkeyMap &&) = delete;

private:
  struct DualkeyHash {
    DualkeyHash(std::size_t AHash, AKey *APointer, std::size_t BHash,
                BKey *BPointer)
        : AKeyHash(AHash), APointer(APointer), BKeyHash(BHash),
          BPointer(BPointer) {}
    std::size_t AKeyHash = 0;
    std::size_t BKeyHash = 0;
    AKey *APointer;
    BKey *BPointer;
    Value *ValuePointer;
  };

  std::vector<DualkeyHash> HashList;
};
} // namespace Tourmaline::Containers
#endif
