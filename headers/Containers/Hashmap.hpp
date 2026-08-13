/*
 * SPDX-FileCopyrightText: Dora "cat" <cat@thenight.club>
 * SPDX-License-Identifier: MPL-2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at http://mozilla.org/MPL/2.0/.
 */
#ifndef GUARD_TOURMALINE_HASHMAP_H
#define GUARD_TOURMALINE_HASHMAP_H
#include "../Concepts.hpp"
#include "../Systems/Logging.hpp"
#include "ContainerOptions.hpp"

#include <cstddef>
#include <vector>

/**
 * @file
 * @brief Built-in hashmap container.
 */

namespace Tourmaline::Containers {
/**
 * @brief A key-value hashmap.
 *
 * @tparam Key Any type that satisfies Tourmaline::Concepts::Hashable.
 * @tparam Value Any type is allowed.
 * @tparam Options See Tourmaline::Containers::HashContainerOptions.
 */
template <Concepts::Hashable Key, typename Value,
          HashContainerOptions Options = {}>
class Hashmap {
public:
  Hashmap() { storage.resize(Options.minimumBucketCount); }
  ~Hashmap() { Clear(); }

  /**
   * @brief Inserts a value with a key to access the value.
   *
   * @param key This will be used to access value.
   * @param value This will be copied into the hashmap to be stored.
   *
   * @return A reference to the inserted value.
   *
   * @warning If you try to insert same key-value pair twice, this will throw
   * a runtime exception (Tourmaline::Systems::Logging::Error).
   */
  Value &Insert(Key key, Value value) {
    if (currentLoadFactor >= Options.loadFactor &&
        currentlyRehashing == false) {
      rehash();
    }
    std::size_t keyHash = std::hash<Key>{}(key),
                keyHashPosition = keyHash % storage.size();

    // Empty bucket
    if (!storage[keyHashPosition].empty()) {
      // Throws
      Systems::Logging::Log("Trying to insert the same key twice! Throwing...",
                            "Hashmap", Systems::Logging::Error, Has(key));
    } else {
      storage[keyHashPosition].reserve(Options.reservedBucketSpace);
    }

    storage[keyHashPosition].emplace_back(key, std::move(value), keyHash);
    currentLoadFactor = (++count) / static_cast<float>(bucketCount);
    return storage[keyHashPosition].back().value;
  }

  /**
   * @brief Removes a value from the map.
   *
   * @param key Key of the key-value pair to remove.
   *
   * @note This function will not deconstruct or erase any data inside pointers.
   * If you are storing pointers with this map, you must manually clear them.
   *
   * @warning If you try to remove a key-value pair that does not exist,
   * this will throw a runtime exception (Tourmaline::Systems::Logging::Error).
   * Please check the availability of a key with Has function first.
   */
  void Remove(const Key &key) {
    std::size_t keyHash = std::hash<Key>{}(key),
                keyHashPosition = keyHash % storage.size();

    // Throws
    Systems::Logging::Log("Trying to remove a non-existant key! Throwing...",
                          "Hashmap", Systems::Logging::Error,
                          storage[keyHashPosition].empty());
    std::erase_if(storage[keyHashPosition],
                  [keyHash, &key](const hashStorage &hash) {
                    return hash.hash == keyHash && hash.key == key;
                  });

    currentLoadFactor = (--count) / static_cast<float>(bucketCount);
    if (currentLoadFactor <= Options.minimizeFactor) {
      rehash();
    }
  }

  /**
   * @brief Checks if a key-value is a member of this map.
   *
   * @param key The key of the key-value pair to check.
   *
   * @return True if the map has the pair, false otherwise.
   */
  [[nodiscard("Unnecessary call of Has function")]]
  bool Has(const Key &key) noexcept {
    std::size_t keyHash = std::hash<Key>{}(key),
                keyHashPosition = keyHash % storage.size();

    // Empty bucket
    if (storage[keyHashPosition].empty()) {
      return false;
    }

    for (const hashStorage &hash : storage[keyHashPosition]) {
      if (hash.hash == keyHash && hash.key == key) {
        return true;
      }
    }

    return false;
  }

  /**
   * @brief Fetches the value of the specified key-value pair.
   *
   * @param key The key of the key-value pair to fetch.
   *
   * @return A reference to the fetched value.
   *
   * @warning If you try to get a key-value pair that does not exist,
   * this will throw a runtime exception (Tourmaline::Systems::Logging::Error).
   * Please check the availability of a key with Has function first.
   */
  [[nodiscard("Unnecessary call of Get function")]]
  Value &Get(const Key &key) {
    std::size_t keyHash = std::hash<Key>{}(key),
                keyHashPosition = keyHash % storage.size();

    Systems::Logging::Log(
        "Trying to access a non-existant bucket for a key! Throwing...",
        "Hashmap", Systems::Logging::Error, storage[keyHashPosition].empty());

    for (hashStorage &hash : storage[keyHashPosition]) {
      if (hash.hash == keyHash && hash.key == key) {
        return hash.value;
      }
    }

    Systems::Logging::Log("Trying to access a non-existant key! Throwing...",
                          "Hashmap", Systems::Logging::Error);
    throw;
  }

  /**
   * @brief Releases all of the values stored.
   *
   * @return Every value stored inside a std::vector.
   *
   * @warning When this function is ran, it will transfer the ownership of the
   * stored data to where it is ran. Therefore this map will own nothing after
   * being ran.
   */
  [[nodiscard("Discarding an expensive operation!")]]
  std::vector<Value> ExtractValuesToArray() {
    std::vector<Value> result;
    result.reserve(count);

    for (bucket &entry : storage) {
      for (hashStorage &hash : entry) {
        result.emplace_back(std::move(hash.value));
      }
      entry.clear();
    }

    count = 0;
    bucketCount = Options.minimumBucketCount;
    std::vector<bucket> newStorage;
    storage.swap(newStorage);
    return result;
  }

  /**
   * @brief Erases all the key-value pairs stored.
   *
   * @note This function will not deconstruct or erase any data inside pointers.
   * If you are storing pointers with this map, you must manually clear them.
   */
  void Clear() noexcept {
    storage.clear();
    count = 0;
  }

  /**
   * @brief Returns the amount of key-value pairs in this map.
   *
   * @return Total amount of active key-value pairs.
   */
  [[nodiscard("Unnecessary call of Count function")]]
  std::size_t Count() noexcept {
    return count;
  }

private:
  bool rehash(std::size_t goalSize = 0) {
    // Minimum
    goalSize = goalSize == 0 ? count : goalSize;
    float wouldBeLoadFactor = goalSize / static_cast<float>(bucketCount);
    if (wouldBeLoadFactor < Options.loadFactor &&
        wouldBeLoadFactor > Options.minimizeFactor) [[unlikely]] {
      return false; // No rehashing is required
    }

    // Putting it closer to minimizeFactor
    std::size_t goalBucketCount = goalSize / preferredLoadFactor;
    if (goalBucketCount < Options.minimumBucketCount) [[unlikely]] {
      goalBucketCount = Options.minimumBucketCount;
    }

    // No need to reallocate
    if (goalBucketCount == bucketCount) {
      return false;
    }

    currentlyRehashing = true;
    std::vector<bucket> oldStorage = std::move(storage);
    storage = std::vector<bucket>();
    storage.resize(goalBucketCount);

    // Repopulate and cleanup
    for (bucket &entry : oldStorage) {
      for (hashStorage &hash : entry) {
        Insert(std::move(hash.key), std::move(hash.value));
      }

      entry.clear();
    }

    // It's necessary to write these again due to insert above
    currentLoadFactor = goalSize / static_cast<float>(goalBucketCount);
    bucketCount = goalBucketCount;
    count = goalSize;
    currentlyRehashing = false;
    return true;
  }

  struct hashStorage {
    Key key;
    Value value;
    std::size_t hash;
  };

  using bucket = std::vector<hashStorage>;
  std::vector<bucket> storage;
  std::size_t count = 0, bucketCount = Options.minimumBucketCount;
  float currentLoadFactor = 0,
        preferredLoadFactor = (Options.loadFactor + Options.minimizeFactor) /
                              Options.leaningFactor;
  bool currentlyRehashing = false; // Lock for Insert in rehash
};
} // namespace Tourmaline::Containers
#endif
