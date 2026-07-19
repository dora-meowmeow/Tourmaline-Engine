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

namespace Tourmaline::Containers {

// Variant of Hashmap that only has key entry, used usually to mark stuff.
template <Concepts::Hashable Entry, HashContainerOptions Options = {}>
class Hashlist {
public:
  Hashlist() { storage.resize(Options.minimumBucketCount); }
  ~Hashlist() { Clear(); }

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

  void Clear() noexcept {
    storage.clear();
    count = 0;
  }

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
