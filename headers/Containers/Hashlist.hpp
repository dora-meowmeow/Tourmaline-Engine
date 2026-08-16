/*
 * SPDX-FileCopyrightText: Dora "cat" <cat@thenight.club>
 * SPDX-License-Identifier: MPL-2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at http://mozilla.org/MPL/2.0/.
 */
#ifndef GUARD_TOURMALINE_LIST_H
#define GUARD_TOURMALINE_LIST_H
#include "../Concepts.hpp"
#include "../Systems/Logging.hpp"
#include "ContainerOptions.hpp"

#include <cstddef>
#include <vector>

/**
 * @file
 * @brief A key-only variant of a hashmap.
 */

namespace Tourmaline::Containers {

/**
 * @brief Key-only variant of Tourmaline::Containers::Hashmap.
 *
 * @tparam Entry Any type that satisfies Tourmaline::Concepts::Hashable.
 * @tparam Options See Tourmaline::Containers::HashContainerOptions.
 *
 * This variant is especially useful, if all you want is a group/set.
 */
template <Concepts::Hashable Entry, HashContainerOptions Options = {}>
class Hashlist {
public:
  Hashlist() { storage.resize(Options.minimumBucketCount); }
  ~Hashlist() { Clear(); }

  /**
   * @brief Inserts an entry into the list.
   *
   * @param entry Entry to be copied into the list.
   *
   * @warning If you try to insert same entry twice, this will throw
   * a runtime exception (Tourmaline::Systems::Logging::Error).
   */
  void Insert(Entry entry) {
    if (currentLoadFactor >= Options.loadFactor &&
        currentlyRehashing == false) {
      rehash();
    }
    std::size_t entryHash = std::hash<Entry>{}(entry),
                entryHashPosition = entryHash % storage.size();

    // Empty bucket
    if (!storage[entryHashPosition].empty()) {
      // Throws
      Systems::Logging::Log(
          "Trying to insert the same entry twice! Throwing...", "Hashmap",
          Systems::Logging::Error, Has(entry));
    } else {
      storage[entryHashPosition].reserve(Options.reservedBucketSpace);
    }

    storage[entryHashPosition].emplace_back(entry, entryHash);
    currentLoadFactor = (++count) / static_cast<float>(bucketCount);
  }

  /**
   * @brief Removes an entry from the list.
   *
   * @param entry Entry to be removed from the list.
   *
   * @note This function will not deconstruct or erase any data inside pointers.
   * If you are storing pointers with this list, you must manually clear them.
   *
   * @warning If you try to remove an entry that does not exist,
   * this will throw a runtime exception (Tourmaline::Systems::Logging::Error).
   * Please check the existence of the entry with the Has function first.
   */
  void Remove(const Entry &entry) {
    std::size_t entryHash = std::hash<Entry>{}(entry),
                entryHashPosition = entryHash % storage.size();
    // Throws
    Systems::Logging::Log("Trying to remove a non-existant entry! Throwing...",
                          "Hashmap", Systems::Logging::Error,
                          storage[entryHashPosition].empty());

    std::erase_if(storage[entryHashPosition],
                  [entryHash, &entry](const hashStorage &hash) {
                    return hash.hash == entryHash && hash.entry == entry;
                  });

    currentLoadFactor = (--count) / static_cast<float>(bucketCount);
    if (currentLoadFactor <= Options.minimizeFactor) {
      rehash();
    }
  }

  /**
   * @brief Checks if an entry is a member of this list.
   *
   * @param entry Entry to check.
   *
   * @return True if the list has it, false otherwise.
   */
  [[nodiscard("Unnecessary call of Has function")]]
  bool Has(const Entry &entry) noexcept {
    std::size_t entryHash = std::hash<Entry>{}(entry),
                entryHashPosition = entryHash % storage.size();

    // Empty bucket
    if (storage[entryHashPosition].empty()) {
      return false;
    }

    for (const hashStorage &hash : storage[entryHashPosition]) {
      if (hash.hash == entryHash && hash.entry == entry) {
        return true;
      }
    }

    return false;
  }
  /**
   * @brief Releases all of the entries stored.
   *
   * @return Every entru stored moved inside a std::vector.
   *
   * @warning When this function is run, it will transfer ownership of the
   * stored data to where it is run. Therefore this map will own nothing after
   * the function runs.
   */
  [[nodiscard("Unnecessary call of ExtractAllEntries function")]]
  std::vector<Entry> ExtractAllEntries() {
    std::vector<Entry> result;
    result.reserve(count);

    for (bucket &entry : storage) {
      for (hashStorage &hash : entry) {
        result.emplace_back(std::move(hash.entry));
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
   * @breif Outputs all of the entries in a list as a std::vector.
   *
   * @return Copies of entries inside a vector.
   */
  [[nodiscard("Unnecessary call of ListAllEntries function")]]
  std::vector<Entry> ListAllEntries() {
    std::vector<Entry> result;
    result.reserve(count);

    for (bucket &entry : storage) {
      for (hashStorage &hash : entry) {
        result.push_back(hash.entry);
      }
    }

    return result;
  }

  /**
   * @brief Erases all the elements.
   *
   * @note This function will not deconstruct or erase any data inside pointers.
   * If you are storing pointers with this list, you must manually clear them.
   */
  void Clear() noexcept {
    storage.clear();
    count = 0;
  }

  /**
   * @brief Returns the amount of entries in this list.
   *
   * @return Total amount of active entries.
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
        Insert(std::move(hash.entry));
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
    Entry entry;
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

}; // namespace Tourmaline::Containers
#endif
