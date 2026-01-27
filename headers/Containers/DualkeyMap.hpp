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

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <optional>
#include <utility>
#include <variant>
#include <vector>

namespace Tourmaline::Containers {
template <Hashable AKey, Hashable BKey, typename Value,
          uint64_t baseReservation = 2048>
class DualkeyMap {
public:
  using ResultPair =
      std::pair<std::variant<std::monostate, std::reference_wrapper<AKey>,
                             std::reference_wrapper<BKey>>,
                Value &>;

  DualkeyMap() { HashList.reserve(baseReservation); }
  ~DualkeyMap() {
    // I'm sure there is a better way to do this
    for (DualkeyHash *hash : HashList) {
      delete hash;
    }
  }

  // Insertion
  void Insert(AKey firstKey, BKey secondKey, Value value) {
    std::size_t firstKeyHash = std::hash<AKey>{}(firstKey);
    std::size_t secondKeyHash = std::hash<BKey>{}(secondKey);
    HashList.push_back(new DualkeyHash(firstKeyHash, std::move(firstKey),
                                       secondKeyHash, std::move(secondKey),
                                       std::move(value)));
  }

  // Indexing
  std::vector<ResultPair> Query(std::optional<AKey> firstKey,
                                std::optional<BKey> secondKey) {
    bool isFirstKeyGiven = firstKey.has_value();
    bool isSecondKeyGiven = secondKey.has_value();

    if (!isFirstKeyGiven && !isSecondKeyGiven) [[unlikely]] {
      Systems::Logging::Log("Failed to index! Dualkey maps require at least 1 "
                            "key to be given, returning an empty vector.",
                            "Dualkey Map", Systems::Logging::LogLevel::Warning);
      return {};
    }
    std::size_t firstKeyHash =
        isFirstKeyGiven ? std::hash<AKey>{}(firstKey.value()) : 0;
    std::size_t secondKeyHash =
        isSecondKeyGiven ? std::hash<BKey>{}(secondKey.value()) : 0;

    std::vector<ResultPair> finishedQuery{};

    uint8_t stateOfIndexing = isFirstKeyGiven + (isSecondKeyGiven << 1);
    // Putting hash checks first to benefit from short circuits
    for (DualkeyHash *hash : HashList) {
      switch (stateOfIndexing) {
      case 1: // Only first key is given
        if (firstKeyHash == hash->firstKeyHash &&
            firstKey.value() == hash->firstKey) {
          finishedQuery.emplace_back(
              std::reference_wrapper<BKey>{hash->secondKey}, hash->value);
        }
        continue;
      case 2: // Only second key is given
        if (secondKeyHash == hash->secondKeyHash &&
            secondKey.value() == hash->secondKey) {
          finishedQuery.emplace_back(
              std::reference_wrapper<AKey>{hash->firstKey}, hash->value);
        }
        continue;
      case 3: // Both are given
        if (firstKeyHash == hash->firstKeyHash &&
            secondKeyHash == hash->secondKeyHash &&
            firstKey.value() == hash->firstKey &&
            secondKey.value() == hash->secondKey) {
          finishedQuery.emplace_back(std::monostate{}, hash->value);
          break;
        }
        continue;
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
    DualkeyHash(std::size_t firstKeyHash, AKey &&firstKey,
                std::size_t secondKeyHash, BKey &&secondKey, Value &&value)
        : firstKeyHash(firstKeyHash), firstKey(std::move(firstKey)),
          secondKeyHash(secondKeyHash), secondKey(std::move(secondKey)),
          value(std::move(value)) {}

    std::size_t firstKeyHash = 0;
    std::size_t secondKeyHash = 0;
    AKey firstKey;
    BKey secondKey;
    Value value;
  };

  // It makes more sense to store the individual hash
  std::vector<DualkeyHash *> HashList;
};
} // namespace Tourmaline::Containers
#endif
