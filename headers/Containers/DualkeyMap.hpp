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
#include <stack>
#include <tuple>
#include <utility>
#include <variant>
#include <vector>

namespace Tourmaline::Containers {
template <Hashable AKey, Hashable BKey, typename Value,
          uint64_t baseReservation = 2048, double maxTombstoneRatio = 0.25>
class DualkeyMap {
public:
  using QueryResult =
      std::pair<std::variant<std::monostate, std::reference_wrapper<const AKey>,
                             std::reference_wrapper<const BKey>>,
                Value &>;
  using Entry = std::tuple<const AKey &, const BKey &, Value &>;

  DualkeyMap() { hashList.reserve(baseReservation); }
  ~DualkeyMap() {
    // I'm sure there is a better way to do this
    for (DualkeyHash *hash : hashList) {
      if (hash != nullptr) [[likely]] {
        delete hash;
      }
    }
  }

  Entry insert(AKey firstKey, BKey secondKey, Value value) {
    std::size_t firstKeyHash = std::hash<AKey>{}(firstKey);
    std::size_t secondKeyHash = std::hash<BKey>{}(secondKey);
    DualkeyHash *hash =
        new DualkeyHash(firstKeyHash, std::move(firstKey), secondKeyHash,
                        std::move(secondKey), std::move(value));

    if (graveyard.empty()) {
      hashList.push_back(hash);
    } else {
      hashList[graveyard.top()] = hash;
      graveyard.pop();
    }

    return {hash->firstKey, hash->secondKey, hash->value};
  }

  std::size_t remove(std::optional<AKey> firstKey,
                     std::optional<BKey> secondKey) {
    bool isFirstKeyGiven = firstKey.has_value();
    bool isSecondKeyGiven = secondKey.has_value();

    if (!isFirstKeyGiven && !isSecondKeyGiven) [[unlikely]] {
      Systems::Logging::Log("Failed to Delete! Dualkey maps require at least 1 "
                            "key to be given, doing nothing.",
                            "Dualkey Map", Systems::Logging::LogLevel::Warning);
      return 0;
    }

    std::size_t firstKeyHash =
        isFirstKeyGiven ? std::hash<AKey>{}(firstKey.value()) : 0;
    std::size_t secondKeyHash =
        isSecondKeyGiven ? std::hash<BKey>{}(secondKey.value()) : 0;
    std::size_t index = 0, amountDeleted = 0;
    uint8_t stateOfIndexing = isFirstKeyGiven + (isSecondKeyGiven << 1);
    for (DualkeyHash *hash : hashList) {

      // Tombstone
      if (hash == nullptr) [[unlikely]] {
        continue;
      }

      switch (stateOfIndexing) {
      case 1: // Only first key is given
        if (firstKeyHash == hash->firstKeyHash &&
            firstKey.value() == hash->firstKey) {
          delete hash;
          hashList[index] = nullptr;
          graveyard.push(index);
          ++amountDeleted;
        }
        break;

      case 2: // Only second key is given
        if (secondKeyHash == hash->secondKeyHash &&
            secondKey.value() == hash->secondKey) {
          delete hash;
          hashList[index] = nullptr;
          graveyard.push(index);
          ++amountDeleted;
        }
        break;

      case 3:
        if (firstKeyHash == hash->firstKeyHash &&
            secondKeyHash == hash->secondKeyHash &&
            firstKey.value() == hash->firstKey &&
            secondKey.value() == hash->secondKey) {
          delete hash;
          hashList[index] = nullptr;
          graveyard.push(index);
          return 1;
        }
        break;
      }
      ++index;
    }
    return amountDeleted;
  }

  [[nodiscard("Discarding an expensive query!")]]
  std::vector<QueryResult> query(std::optional<AKey> firstKey,
                                 std::optional<BKey> secondKey) {
    bool isFirstKeyGiven = firstKey.has_value();
    bool isSecondKeyGiven = secondKey.has_value();

    if (!isFirstKeyGiven && !isSecondKeyGiven) [[unlikely]] {
      Systems::Logging::Log("Failed to Query! Dualkey maps require at least 1 "
                            "key to be given, returning an empty vector.",
                            "Dualkey Map", Systems::Logging::LogLevel::Warning);
      return {};
    }
    std::size_t firstKeyHash =
        isFirstKeyGiven ? std::hash<AKey>{}(firstKey.value()) : 0;
    std::size_t secondKeyHash =
        isSecondKeyGiven ? std::hash<BKey>{}(secondKey.value()) : 0;

    std::vector<QueryResult> finishedQuery{};

    uint8_t stateOfIndexing = isFirstKeyGiven + (isSecondKeyGiven << 1);
    // Putting hash checks first to benefit from short circuits
    for (DualkeyHash *hash : hashList) {
      // Tombstone
      if (hash == nullptr) [[unlikely]] {
        continue;
      }

      switch (stateOfIndexing) {
      case 1: // Only first key is given
        if (firstKeyHash == hash->firstKeyHash &&
            firstKey.value() == hash->firstKey) {
          finishedQuery.emplace_back(std::cref(hash->secondKey), hash->value);
        }
        continue;
      case 2: // Only second key is given
        if (secondKeyHash == hash->secondKeyHash &&
            secondKey.value() == hash->secondKey) {
          finishedQuery.emplace_back(std::cref(hash->firstKey), hash->value);
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

  void
  scan(std::function<bool(const AKey &, const BKey &, Value &)> scanFunction) {
    for (DualkeyHash *hash : hashList) {
      if (hash == nullptr) {
        continue;
      }
      if (scanFunction(hash->firstKey, hash->secondKey, hash->value)) {
        return;
      }
    }
  }

  [[nodiscard]]
  std::size_t count() {
    return hashList.size() - graveyard.size();
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
        : firstKeyHash(firstKeyHash), secondKeyHash(secondKeyHash),
          firstKey(std::move(firstKey)), secondKey(std::move(secondKey)),
          value(std::move(value)) {}

    const std::size_t firstKeyHash;
    const std::size_t secondKeyHash;
    const AKey firstKey;
    const BKey secondKey;
    mutable Value value;
  };

  // It makes more sense to store the individual hash
  std::vector<DualkeyHash *> hashList;
  std::stack<std::size_t> graveyard;
};
} // namespace Tourmaline::Containers
#endif
