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
#include "../Concepts.hpp"
#include "../Systems/Logging.hpp"
#include "Hashmap.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <optional>
#include <stack>
#include <tuple>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

namespace Tourmaline::Containers {
template <Concepts::Hashable AKey, Concepts::Hashable BKey, typename Value,
          uint64_t baseReservation = 2048>
class DualkeyMap {
public:
  // Return Types
  template <typename OppositeKey, std::size_t keyCount>
    requires Concepts::Either<OppositeKey, AKey, BKey>
  struct MultiQueryResult {
    // Having to use pointers here over references was not fun
    // but it was for greater good
    const OppositeKey *oppositeKey;
    std::size_t howManyFound = 1;
    std::array<Value *, keyCount> valueQueryResults;
  };

  using QueryResult =
      std::pair<std::variant<std::monostate, std::reference_wrapper<const AKey>,
                             std::reference_wrapper<const BKey>>,
                Value &>;
  using Entry = std::tuple<const AKey &, const BKey &, Value &>;

  // Construct/Destruct
  DualkeyMap() { hashList.reserve(baseReservation); }
  ~DualkeyMap() {
    // I'm sure there is a better way to do this
    for (DualkeyHash *hash : hashList) {
      if (hash != nullptr) [[likely]] {
        delete hash;
      }
    }
  }

  // No copying due to the container expected to be the sole
  // owner of the data
  DualkeyMap(const DualkeyMap &) = delete;
  DualkeyMap &operator=(const DualkeyMap &) = delete;

  // Public controls
  Entry Insert(AKey firstKey, BKey secondKey, Value value) {
    DualkeyHash *hash = new DualkeyHash(std::move(firstKey),
                                        std::move(secondKey), std::move(value));

    if (graveyard.empty()) {
      hashList.push_back(hash);
    } else {
      hashList[graveyard.top()] = hash;
      graveyard.pop();
    }

    return {hash->firstKey, hash->secondKey, hash->value};
  }

  std::size_t Remove(std::optional<AKey> firstKey,
                     std::optional<BKey> secondKey) {
    bool isFirstKeyGiven = firstKey.has_value();
    bool isSecondKeyGiven = secondKey.has_value();

    if (!isFirstKeyGiven && !isSecondKeyGiven) [[unlikely]] {
      Systems::Logging::Log(
          "Failed to Delete! DualkeyMap::Delete require at least 1 "
          "key to be given! Terminating",
          "Dualkey Map", Systems::Logging::LogLevel::Critical);
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

      case 3: // Both given
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

  [[nodiscard]]
  std::size_t Count() {
    return hashList.size() - graveyard.size();
  }

  // Queries
  [[nodiscard("Discarding an expensive query!")]]
  std::vector<QueryResult> Query(std::optional<AKey> firstKey,
                                 std::optional<BKey> secondKey) {
    bool isFirstKeyGiven = firstKey.has_value();
    bool isSecondKeyGiven = secondKey.has_value();

    if (!isFirstKeyGiven && !isSecondKeyGiven) [[unlikely]] {
      Systems::Logging::Log(
          "Failed to Query! DualkeyMap::Query require at least 1 "
          "key to be given! Terminating",
          "Dualkey Map", Systems::Logging::LogLevel::Critical);
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

  template <typename Key,
            typename OppositeKey = Concepts::OppositeOf<Key, AKey, BKey>,
            std::size_t keyCount>
    requires Concepts::Either<Key, AKey, BKey>
  [[nodiscard("Discarding a very expensive query!")]]
  std::vector<MultiQueryResult<OppositeKey, keyCount>>
  QueryWithAll(const Key (&keys)[keyCount]) {
    std::vector<MultiQueryResult<OppositeKey, keyCount>> queryResult =
        queryWithMany<Key>(keys);
    std::erase_if(
        queryResult,
        [](const MultiQueryResult<OppositeKey, keyCount> &queryRecord) {
          return queryRecord.howManyFound != keyCount;
        });
    return queryResult;
  }

  void Scan(std::function<bool(const std::size_t firstKeyHash,
                               const std::size_t secondKeyHash, Value &value)>
                scanFunction) {
    for (DualkeyHash *hash : hashList) {
      if (hash == nullptr) {
        continue;
      }
      if (scanFunction(hash->firstKeyHash, hash->secondKeyHash, hash->value)) {
        return;
      }
    }
  }

  void Scan(std::function<bool(const AKey &firstKey, const BKey &secondKey,
                               Value &value)>
                scanFunction) {
    for (DualkeyHash *hash : hashList) {
      if (hash == nullptr) {
        continue;
      }
      if (scanFunction(hash->firstKey, hash->secondKey, hash->value)) {
        return;
      }
    }
  }

private:
  // Interal data structures
  struct DualkeyHash {
    DualkeyHash(AKey &&firstKey, BKey &&secondKey, Value &&value)
        : firstKey(std::move(firstKey)), secondKey(std::move(secondKey)),
          firstKeyHash(std::hash<AKey>{}(this->firstKey)),
          secondKeyHash(std::hash<BKey>{}(this->secondKey)),
          value(std::move(value)) {}

    const AKey firstKey;
    const BKey secondKey;
    const std::size_t firstKeyHash;
    const std::size_t secondKeyHash;
    mutable Value value;
  };

  // Actual data
  std::vector<DualkeyHash *> hashList;
  std::stack<std::size_t> graveyard;

  // Interal querying
  template <typename Key,
            typename OppositeKey = Concepts::OppositeOf<Key, AKey, BKey>,
            std::size_t keyCount>
  inline std::vector<MultiQueryResult<OppositeKey, keyCount>>
  queryWithMany(const Key (&keys)[keyCount]) {
    constexpr bool searchingInFirstKey = std::is_same_v<Key, AKey>;

    // I really can't wait for C++26 contracts
    if constexpr (keyCount == 0) {
      Systems::Logging::Log("Failed to Query! QueryWithAll require at least 2 "
                            "key to be given, zero was given! Terminating",
                            "Dualkey Map",
                            Systems::Logging::LogLevel::Critical);
    }

    // Hoping this never ever gets triggered :sigh:
    if constexpr (keyCount == 1) {
      Systems::Logging::Log("QueryWithAll should not be used for single key "
                            "entry! Please use Query for this instead.",
                            "Dualkey Map", Systems::Logging::LogLevel::Error);
    }

    // While we don't necessary need the hashes,
    // it just helps us tremendously benefit from short circuit checks
    std::array<std::size_t, keyCount> keyHashes;
    for (uint64_t index = 0; index < keyCount; index++) {
      keyHashes[index] = std::hash<Key>{}(keys[index]);
    }

    uint64_t hashToCompare;
    Key *keyToCompare;
    OppositeKey *oppositeKey;

    Containers::Hashmap<OppositeKey, MultiQueryResult<OppositeKey, keyCount>,
                        8.0f, 2048, 0.01f> // Aggressive hashmap :o
        queryResults;

    for (DualkeyHash *hash : hashList) {
      // Tombstone
      if (hash == nullptr) {
        continue;
      }

      // The hell of doing 2 conditions with similar logics in
      // the same logical block
      if constexpr (searchingInFirstKey) {
        hashToCompare = hash->firstKeyHash;
        keyToCompare = const_cast<AKey *>(&hash->firstKey);
        oppositeKey = const_cast<BKey *>(&hash->secondKey);
      } else {
        hashToCompare = hash->secondKeyHash;
        keyToCompare = const_cast<BKey *>(&hash->secondKey);
        oppositeKey = const_cast<AKey *>(&hash->firstKey);
      }

      // The code above was done to make this code more uniform
      for (uint64_t index = 0; index < keyCount; index++) {
        if (keyHashes[index] == hashToCompare && keys[index] == *keyToCompare) {
          if (queryResults.Has(*oppositeKey)) {
            auto &entry = queryResults.Get(*oppositeKey);
            entry.valueQueryResults[index] = &hash->value;
            ++entry.howManyFound;
            break;
          }

          queryResults
              .Insert(*oppositeKey,
                      MultiQueryResult<OppositeKey, keyCount>(oppositeKey))
              .valueQueryResults[index] = &hash->value;
        }
      }
    }

    return queryResults.ExtractValuesToArray();
  }
};
} // namespace Tourmaline::Containers
#endif
