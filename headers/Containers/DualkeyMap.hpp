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
#include "ContainerOptions.hpp"
#include "Corrade/Tags.h"
#include "Hashmap.hpp"

#include "Corrade/Containers/Array.h"

#include <algorithm>
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

/**
 * @file
 * @brief A hashmap that uses two keys to store a value.
 */

namespace Tourmaline::Containers {

/**
 * @brief A hashmap with two keys instead of one.
 *
 * @tparam AKey Any type that satisfies Tourmaline::Concepts::Hashable.
 * @tparam BKey Any type that satisfies Tourmaline::Concepts::Hashable.
 * @tparam Value Any type is allowed.
 * @tparam Options See Tourmaline::Containers::HashContainerOptions.
 *
 * Two or more entries can share the same AKey or the same BKey, however no two
 * entries can share the same AKey + BKey combo. This allows you to fetch for
 * only AKey/BKey with a specific value.
 *
 * For example: If your AKey is userID and BKey is OrderID, you can query for
 * userID 10. Which will return every entry with a userID of 10.
 */
template <Concepts::Hashable AKey, Concepts::Hashable BKey, typename Value>
class DualkeyMap {
public:
  // Return Types
  /**
   * @brief Similar to Tourmaline::Containers::DualKeyMap::QueryResult for multi
   * queries.
   *
   * @tparam OppositeKey must be either AKey or BKey. Opposite Key to the
   * queried key.
   *
   * Tourmaline::Containers::DualKeyMap::QueryWithAll returns this type.
   */
  template <typename OppositeKey>
    requires Concepts::Either<OppositeKey, AKey, BKey>
  struct MultiQueryResult {
    // Having to use pointers here over references was not fun
    // but it was for greater good

    /// @brief The opposite key to the key used to query.
    const OppositeKey *oppositeKey;

    /// @brief The results of the query stored as pointers to the value.
    Corrade::Containers::Array<Value *> valueQueryResults;

    /// @brief The count of found values.
    std::size_t howManyFound = 1;
  };

  /**
   * @brief Results from Tourmaline::Containers::DualKeyMap::Query.
   *
   * This is an std::pair with 3 possible values for the first element.
   * - If you queried with AKey, then the first element is the associated BKey.
   * - If you queried with BKey, then the first element is the associated AKey.
   * - If you queried with both AKey and BKey, then the first element is set to
   * std::monostate (i.e. empty).
   *
   * The second element is always a reference to the value.
   */
  using QueryResult =
      std::pair<std::variant<std::monostate, std::reference_wrapper<const AKey>,
                             std::reference_wrapper<const BKey>>,
                Value &>;
  /**
   * @brief Returned by Tourmaline::Containers::DualKeyMap::Insert.
   * Allows you to edit the inserted value.
   */
  using Entry = std::tuple<const AKey &, const BKey &, Value &>;

  // Construct/Destruct
  DualkeyMap(DualKeyMapOptions options = {}) : Options(options) {
    hashList.reserve(Options.baseReservation);
  }
  ~DualkeyMap() {
    // I'm sure there is a better way to do this
    for (DualkeyHash *hash : hashList) {
      if (hash != nullptr) [[likely]] {
        delete hash;
      }
    }
  }

  /// @warning No copying as the container is expected to be the sole
  /// owner of the data.
  DualkeyMap(const DualkeyMap &) = delete;

  /// @warning No copying as the container is expected to be the sole
  /// owner of the data.
  DualkeyMap &operator=(const DualkeyMap &) = delete;

  // Public controls

  /**
   * @brief Inserts the AKey-BKey-value entry into the DualKeyMap.
   *
   * @param firstKey This is expected to be a value of AKey.
   * @param secondKey This is expected to be a value of BKey.
   * @param value Value to be stored.
   *
   * @return The entry itself as a reference, so as not to require fetching
   * after insertion.
   */
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

  /**
   * @brief Removes a single entry, or entire group of entries.
   *
   * @param firstKey Can either be std::nullopt_t or an actual AKey value.
   * @param secondKey Can either be std::nullopt_t or an actual BKey value.
   *
   * @return Amount of elements removed.
   *
   * @note Both arguments can be specified, if you want to remove a specific
   * entry.
   * @warning If both arguments are std::nullopt_t, then the software will
   * terminate(Tourmaline::Systems::Logging::Critical).
   */
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
        ++index;
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

  /**
   * @brief Returns the amount of entries in this DualkeyMap.
   *
   * @return Total amount of active entries.
   */
  [[nodiscard]]
  std::size_t Count() {
    return hashList.size() - graveyard.size();
  }

  /**
   * @brief Erases all the elements.
   *
   * @note This function will not deconstruct or erase any data inside pointers.
   * If you are storing pointers with this list, you must manually clear them.
   */
  void Clear() noexcept {
    // I'm sure there is a better way to do this
    for (DualkeyHash *hash : hashList) {
      if (hash != nullptr) [[likely]] {
        delete hash;
      }
    }
    hashList.clear();

    // wth this is a thing???
    std::ignore = graveyard.empty();
  }

  // Queries
  /**
   * @brief Queries for a single entry, or entire group of entries.
   *
   * @param firstKey Can either be std::nullopt_t or an actual AKey value.
   * @param secondKey Can either be std::nullopt_t or an actual BKey value.
   *
   * @return Result of the query.
   *
   * @note Both arguments can be specified, in which case std::vector will
   * either have 0 elements or 1 element, depending on whether the query could
   * find the entry.
   *
   * @warning If both arguments are std::nullopt_t, then the software will
   * terminate(Tourmaline::Systems::Logging::Critical).
   */
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

  /**
   * @brief Queries for a single entry, or entire group of entries.
   *
   * @tparam Key Must be either AKey or BKey.
   * @tparam OppositeKey This should be automatically filled. However, if it is
   * not, it should be the opposite key to Key.
   *
   * @param keys List of values of Key to be used to query.
   * @param ignoreChecks Highly unadvised to set to true. This will disable
   * internal checks for querying for a single entry, or with a single value of
   * Key.
   *
   * @return Result of the query.
   *
   * This will query for several specified values of Key. It will return all
   * entries for which the queried Key matches these values, if and only if the
   * respective OppositeKey has entries matching **every** value specified for
   * Key.
   *
   * For example: if the queried Key is AKey and values 2 and 11 are queried:
   * Every entry with a BKey that has entries with AKey values 2 and 11 will be
   * returned. Otherwise they will be omitted.
   */
  template <typename Key,
            typename OppositeKey = Concepts::OppositeOf<Key, AKey, BKey>>
    requires Concepts::Either<Key, AKey, BKey>
  [[nodiscard("Discarding a very expensive query!")]]
  std::vector<MultiQueryResult<OppositeKey>>
  QueryWithAll(Corrade::Containers::ArrayView<Key> keys,
               bool ignoreChecks = false) {
    std::vector<MultiQueryResult<OppositeKey>> queryResult =
        queryWithMany<Key>(keys, ignoreChecks);

    std::erase_if(queryResult,
                  [keyCount = keys.size()](
                      const MultiQueryResult<OppositeKey> &queryRecord) {
                    return queryRecord.howManyFound != keyCount;
                  });
    return queryResult;
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
    for (DualkeyHash *hash : hashList) {
      if (hash == nullptr) {
        continue;
      }
      if (scanFunction(hash->firstKeyHash, hash->secondKeyHash, hash->value)) {
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
  DualKeyMapOptions Options;
  std::vector<DualkeyHash *> hashList;
  std::stack<std::size_t> graveyard;

  // Interal querying
  template <typename Key,
            typename OppositeKey = Concepts::OppositeOf<Key, AKey, BKey>>
  inline std::vector<MultiQueryResult<OppositeKey>>
  queryWithMany(Corrade::Containers::ArrayView<Key> keys,
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
          if (queryResults.Has(*oppositeKey)) [[likely]] {
            auto &entry = queryResults.Get(*oppositeKey);
            entry.valueQueryResults[index] = &hash->value;
            ++entry.howManyFound;
            break;
          }

          queryResults
              .Insert(*oppositeKey, {oppositeKey,
                                     Corrade::Containers::Array<Value *>{
                                         Corrade::NoInit, keyCount}})
              .valueQueryResults[index] = &hash->value;
        }
      }
    }

    return queryResults.ExtractAllValues();
  }
};
} // namespace Tourmaline::Containers
#endif
