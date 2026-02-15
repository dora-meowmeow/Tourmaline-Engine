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

#include <array>
#include <cmath>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <format>
#include <functional>
#include <optional>
#include <stack>
#include <tuple>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

namespace Tourmaline::Containers {
template <Hashable AKey, Hashable BKey, typename Value,
          uint64_t baseReservation = 2048>
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

  template <typename Key, std::size_t keyCount>
    requires(std::same_as<Key, AKey> || std::same_as<Key, BKey>)
  [[nodiscard("Discarding a very expensive query!")]]
  int QueryWithAll(const Key (&keys)[keyCount]) {
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

    std::vector<MultiQueryResult<
        std::conditional_t<searchingInFirstKey, BKey, AKey>, keyCount>>
        queryResults;
    uint64_t hashToCompare;
    Key *keyToCompare;
    std::conditional_t<searchingInFirstKey, BKey *, AKey *> resultKey;
    for (DualkeyHash *hash : hashList) {
      // The hell of doing 2 conditions with similar logics in
      // the same logical block
      if constexpr (searchingInFirstKey) {
        hashToCompare = hash->firstKeyHash;
        keyToCompare = const_cast<AKey *>(&hash->firstKey);
        resultKey = const_cast<BKey *>(&hash->secondKey);
      } else {
        hashToCompare = hash->secondKeyHash;
        keyToCompare = const_cast<BKey *>(&hash->secondKey);
        resultKey = const_cast<AKey *>(&hash->firstKey);
      }

      // The code above was done to make this code more uniform
      for (uint64_t index = 0; index < keyCount; index++) {
        if (keyHashes[index] == hashToCompare && keys[index] == *keyToCompare) {

          bool doesExist = false;
          for (auto &queryRecord : queryResults) {
            if (*queryRecord.resultKey == *resultKey) {
              queryRecord.valueQueryResults[index] = &hash->value;
              ++queryRecord.howManyFound;
              doesExist = true;
              break;
            }
          }

          if (doesExist) {
            break;
          }

          // Since the result record is not present
          // we have to make it
          queryResults.emplace_back();
          auto &newRecord = queryResults.back();
          newRecord.resultKey = resultKey;
          newRecord.valueQueryResults[index] = &hash->value;
        }
      }
    }

    for (const auto &queryRecord : queryResults) {
      Systems::Logging::Log(
          std::format("Opposite = {}, found = {}",
                      reinterpret_cast<uint64_t>(queryRecord.resultKey),
                      queryRecord.howManyFound),
          "DKM", Systems::Logging::LogLevel::Info,
          queryRecord.howManyFound == keyCount);
    }

    return 0;
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

  [[nodiscard]]
  std::size_t Count() {
    return hashList.size() - graveyard.size();
  }

  // No copying due to the container expected to be the sole
  // owner of the data
  DualkeyMap(const DualkeyMap &) = delete;
  DualkeyMap &operator=(const DualkeyMap &) = delete;

private:
  template <typename OppositeKey, std::size_t keyCount>
  struct MultiQueryResult {
    OppositeKey *resultKey = nullptr;
    std::size_t howManyFound = 1;
    std::array<Value *, keyCount> valueQueryResults;
  };

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

  // It makes more sense to store the individual hash
  std::vector<DualkeyHash *> hashList;
  std::stack<std::size_t> graveyard;
};
} // namespace Tourmaline::Containers
#endif
