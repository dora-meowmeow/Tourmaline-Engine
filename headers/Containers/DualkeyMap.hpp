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
#include "Corrade/Containers/Array.h"
#include "Corrade/Tags.h"

#include "../Concepts.hpp"
#include "../Systems/Logging.hpp"
#include "ChunkList.hpp"
#include "ContainerOptions.hpp"
#include "Hashmap.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <expected>
#include <functional>
#include <type_traits>
#include <utility>
#include <vector>

/**
 * @file
 * @brief A map that uses two keys to store a value.
 */

namespace Tourmaline::Containers {

template <Concepts::Hashable AKey, Concepts::Hashable BKey, typename Value>
class DualkeyMap {
public:
  // Return Types
  template <typename OppositeKey>
    requires Concepts::Either<OppositeKey, AKey, BKey>
  struct MultiQueryResult {
    // Having to use pointers here over references was not fun
    // but it was for greater good

    const OppositeKey *oppositeKey;
    Corrade::Containers::Array<Value *> valueQueryResults;
    std::size_t howManyFound = 1;
  };

  // Construct/Destruct
  DualkeyMap(DualKeyMapOptions options = {}) : Options(options) {
    // TODO: Add reserve to ChunkList
  }
  ~DualkeyMap() { Clear(); }

  /// @warning No copying as the container is expected to be the sole
  /// owner of the data.
  DualkeyMap(const DualkeyMap &) = delete;

  /// @warning No copying as the container is expected to be the sole
  /// owner of the data.
  DualkeyMap &operator=(const DualkeyMap &) = delete;

  // Public controls
  Value &Insert(AKey firstKey, BKey secondKey, Value value) {
    ChunkListRecord record = hashList.Insert(
        {std::move(firstKey), std::move(secondKey), std::move(value)});
    auto &data = hashList.Get(record);

    return data.value;
  }

  template <typename Key>
    requires Concepts::Either<Key, AKey, BKey>
  std::size_t RemoveKey(const Key &key) {
    std::size_t hash = std::hash<Key>{}(key), amountDeleted = 0;
    bool comparisonResult = false;
    for (auto iterator = hashList.begin(); iterator != hashList.end();
         ++iterator) {
      DualkeyHash &data = *iterator;
      if constexpr (std::is_same_v<AKey, Key>) {
        comparisonResult = hash == data.firstKeyHash && key == data.firstKey;
      } else { // Lame but necessary else usage
        comparisonResult = hash == data.secondKeyHash && key == data.secondKey;
      }

      if (comparisonResult) {
        hashList.Remove(iterator.position);
        ++amountDeleted;
      }
    }

    return amountDeleted;
  }

  bool RemoveValue(const AKey &firstKey, const BKey &secondKey) {
    std::size_t hashA = std::hash<AKey>{}(firstKey);
    std::size_t hashB = std::hash<BKey>{}(secondKey);
    for (auto iterator = hashList.begin(); iterator != hashList.end();
         ++iterator) {
      DualkeyHash &data = *iterator;
      if (hashA == data.firstKeyHash && hashB == data.secondKeyHash &&
          firstKey == data.firstKey && secondKey == data.secondKey) {
        hashList.Remove(iterator.position);
        return true;
      }
    }
    return false;
  }

  [[nodiscard]]
  std::size_t Count() const noexcept {
    return hashList.CountEntries();
  }

  void Clear() noexcept { hashList.Clear(); }

  // Queries
  template <typename Key,
            typename OppositeKey = Concepts::OppositeOf<Key, AKey, BKey>>
    requires Concepts::Either<Key, AKey, BKey>
  [[nodiscard("Discarding an expensive query!")]]
  std::vector<std::pair<const OppositeKey &, Value &>>
  QueryWithKey(const Key &key) {
    std::vector<std::pair<const OppositeKey &, Value &>> finishedQuery{};
    std::size_t keyHash = std::hash<Key>{}(key);

    // Putting hash checks first to benefit from short circuits
    for (DualkeyHash &hash : hashList) {
      if constexpr (std::is_same_v<AKey, Key>) {
        if (keyHash == hash.firstKeyHash && key == hash.firstKey) {
          finishedQuery.emplace_back(hash.secondKey, hash.value);
        }
      } else {
        if (keyHash == hash.secondKeyHash && key == hash.secondKey) {
          finishedQuery.emplace_back(hash.firstKey, hash.value);
        }
      }
    }

    return finishedQuery;
  }

  [[nodiscard("Discarding an expensive query!")]]
  std::expected<std::reference_wrapper<Value>, bool>
  QueryForValue(const AKey &firstKey, const BKey &secondKey) {
    std::size_t aKeyHash = std::hash<AKey>{}(firstKey),
                bKeyHash = std::hash<BKey>{}(secondKey);

    // Putting hash checks first to benefit from short circuits
    for (DualkeyHash &hash : hashList) {
      if (aKeyHash == hash.firstKeyHash && bKeyHash == hash.secondKeyHash &&
          firstKey == hash.firstKey && secondKey == hash.secondKey) {
        return hash.value;
      }
    }

    return std::unexpected(false);
  }

  template <typename Key,
            typename OppositeKey = Concepts::OppositeOf<Key, AKey, BKey>>
    requires Concepts::Either<Key, AKey, BKey>
  [[nodiscard("Discarding a very expensive query!")]]
  std::vector<MultiQueryResult<OppositeKey>>
  QueryWithAll(Corrade::Containers::ArrayView<Key> keys,
               bool ignoreChecks = false) {
    std::vector<MultiQueryResult<OppositeKey>> queryResult =
        QueryWithAny<Key>(keys, ignoreChecks);

    std::erase_if(queryResult,
                  [keyCount = keys.size()](
                      const MultiQueryResult<OppositeKey> &queryRecord) {
                    return queryRecord.howManyFound != keyCount;
                  });
    return queryResult;
  }

  // Interal querying
  template <typename Key,
            typename OppositeKey = Concepts::OppositeOf<Key, AKey, BKey>>
  std::vector<MultiQueryResult<OppositeKey>>
  QueryWithAny(Corrade::Containers::ArrayView<Key> keys,
               bool ignoreChecks = false) {
    constexpr bool searchingInFirstKey = std::is_same_v<Key, AKey>;
    std::size_t keyCount = keys.size();

    // I really can't wait for C++26 contracts
    if (keyCount == 0 && !ignoreChecks) {
      Systems::Logging::Log("Failed to Query! QueryWithAll require at least 2 "
                            "key to be given, zero was given! Terminating",
                            "Dualkey Map",
                            Systems::Logging::LogLevel::Critical);
    }

    // Hoping this never ever gets triggered :sigh:
    if (keyCount == 1 && !ignoreChecks) {
      Systems::Logging::Log("QueryWithAll should not be used for single key "
                            "entry! Please use Query for this instead.",
                            "Dualkey Map", Systems::Logging::LogLevel::Error);
    }

    // While we don't necessary need the hashes,
    // it just helps us tremendously benefit from short circuit checks
    Corrade::Containers::Array<std::size_t> keyHashes{Corrade::NoInit,
                                                      keyCount};
    for (uint64_t index = 0; index < keyCount; index++) {
      keyHashes[index] = std::hash<Key>{}(keys[index]);
    }

    uint64_t hashToCompare;
    Key *keyToCompare;
    OppositeKey *oppositeKey;

    Containers::Hashmap<OppositeKey, MultiQueryResult<OppositeKey>>
        queryResults({8.0f, 0.01f, 2.5f, 2048, 8});

    for (DualkeyHash &hash : hashList) {
      // The hell of doing 2 conditions with similar logics in
      // the same logical block
      if constexpr (searchingInFirstKey) {
        hashToCompare = hash.firstKeyHash;
        keyToCompare = const_cast<AKey *>(&hash.firstKey);
        oppositeKey = const_cast<BKey *>(&hash.secondKey);
      } else {
        hashToCompare = hash.secondKeyHash;
        keyToCompare = const_cast<BKey *>(&hash.secondKey);
        oppositeKey = const_cast<AKey *>(&hash.firstKey);
      }

      // The code above was done to make this code more uniform
      for (uint64_t index = 0; index < keyCount; index++) {
        if (keyHashes[index] == hashToCompare && keys[index] == *keyToCompare) {
          if (queryResults.Has(*oppositeKey)) [[likely]] {
            auto &entry = queryResults.Get(*oppositeKey);
            entry.valueQueryResults[index] = &hash.value;
            ++entry.howManyFound;
            break;
          }

          queryResults
              .Insert(*oppositeKey, {oppositeKey,
                                     Corrade::Containers::Array<Value *>{
                                         Corrade::NoInit, keyCount}})
              .valueQueryResults[index] = &hash.value;
        }
      }
    }

    return queryResults.ExtractAllValues();
  }

  /**
   * @brief Walk through every entry with a function (Hash variant).
   *
   * @param scanFunction This function should have the signature bool(const
   * std::size_t firstKeyHash, const std::size_t secondKeyHash, Value &value).
   *
   * @note When scanFunction returns true then the function will terminate.
   */
  void Scan(std::function<bool(const std::size_t firstKeyHash,
                               const std::size_t secondKeyHash, Value &value)>
                scanFunction) {
    for (DualkeyHash &hash : hashList) {
      if (scanFunction(hash.firstKeyHash, hash.secondKeyHash, hash.value)) {
        return;
      }
    }
  }

  /**
   * @brief Walk through every entry with a function (Value variant).
   *
   * @param scanFunction This function should have the signature bool(const AKey
   * &firstKey, const BKey &secondKey, Value &value)
   *
   * @note When scanFunction returns true then the function will terminate.
   */
  void Scan(std::function<bool(const AKey &firstKey, const BKey &secondKey,
                               Value &value)>
                scanFunction) {
    for (DualkeyHash &hash : hashList) {
      if (scanFunction(hash.firstKey, hash.secondKey, hash.value)) {
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
  DualKeyMapOptions Options;
  Containers::ChunkList<DualkeyHash> hashList;
};
} // namespace Tourmaline::Containers
#endif
