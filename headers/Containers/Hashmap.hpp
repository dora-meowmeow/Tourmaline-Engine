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

#include <cstddef>
#include <vector>

namespace Tourmaline::Containers {
template <Concepts::Hashable Key, typename Value, float loadFactor = 0.75f,
          std::size_t minimumBucketCount = 256, float minimizeFactor = 0.20f>
class Hashmap {
public:
  Hashmap() { storage.resize(minimumBucketCount); }
  ~Hashmap() { Clear(); }

  Value &Insert(Key key, Value value) {
    if (currentLoadFactor >= loadFactor && currentlyRehashing == false) {
      rehash();
    }
    std::size_t keyHash = std::hash<Key>{}(key),
                keyHashPosition = keyHash % storage.size();

    // Empty bucket
    if (storage[keyHashPosition] == nullptr) {
      storage[keyHashPosition] = new std::vector<hashStorage>;
    } else {
      // Throws
      Systems::Logging::Log("Trying to inserting same key twice! Throwing...",
                            "Hashmap", Systems::Logging::LogLevel::Error,
                            Has(key));
    }

    storage[keyHashPosition]->emplace_back(key, std::move(value), keyHash);
    currentLoadFactor = (++count) / static_cast<float>(bucketCount);
    return storage[keyHashPosition]->back().value;
  }

  void Remove(const Key &key) {
    std::size_t keyHash = std::hash<Key>{}(key),
                keyHashPosition = keyHash % storage.size();

    // Throws
    Systems::Logging::Log("Trying to remove a non-existant key! Throwing...",
                          "Hashmap", Systems::Logging::LogLevel::Error,
                          storage[keyHashPosition] == nullptr);
    std::erase_if(*storage[keyHashPosition],
                  [keyHash, &key](const hashStorage &hash) {
                    return hash.hash == keyHash && hash.key == key;
                  });

    currentLoadFactor = (--count) / static_cast<float>(bucketCount);
    if (currentLoadFactor <= minimizeFactor) {
      rehash();
    }
  }

  [[nodiscard("Unnecessary call of Has function")]]
  bool Has(const Key &key) noexcept {
    std::size_t keyHash = std::hash<Key>{}(key),
                keyHashPosition = keyHash % storage.size();

    // Empty bucket
    if (storage[keyHashPosition] == nullptr) {
      return false;
    }

    for (const hashStorage &hash : *storage[keyHashPosition]) {
      if (hash.hash == keyHash && hash.key == key) {
        return true;
      }
    }

    return false;
  }

  [[nodiscard("Unnecessary call of Get function")]]
  Value &Get(const Key &key) {
    std::size_t keyHash = std::hash<Key>{}(key),
                keyHashPosition = keyHash % storage.size();

    Systems::Logging::Log(
        "Trying to access a non-existant bucket for a key! Throwing...",
        "Hashmap", Systems::Logging::LogLevel::Error,
        storage[keyHashPosition] == nullptr);

    for (hashStorage &hash : *storage[keyHashPosition]) {
      if (hash.hash == keyHash && hash.key == key) {
        return hash.value;
      }
    }

    Systems::Logging::Log("Trying to access a non-existant key! Throwing...",
                          "Hashmap", Systems::Logging::LogLevel::Error);
  }

  void Clear() noexcept {
    for (bucket *entry : storage) {
      if (entry == nullptr) {
        continue;
      }

      entry->clear();
      delete entry;
    }
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
    if (wouldBeLoadFactor < loadFactor && wouldBeLoadFactor > minimizeFactor)
        [[unlikely]] {
      return false; // No rehashing is required
    }

    // Putting it closer to minimizeFactor
    std::size_t goalBucketCount =
        goalSize / ((loadFactor + minimizeFactor) / 2.5); // Magic number
    if (goalBucketCount < minimumBucketCount) [[unlikely]] {
      goalBucketCount = minimumBucketCount;
    }

    // No need to reallocate
    if (goalBucketCount == bucketCount) {
      return false;
    }

    currentlyRehashing = true;
    std::vector<bucket *> oldStorage = std::move(storage);
    storage = std::vector<bucket *>();
    storage.resize(goalBucketCount);

    // Repopulate and cleanup
    for (bucket *entry : oldStorage) {
      if (entry == nullptr) {
        continue;
      }

      for (const hashStorage &hash : *entry) {
        Insert(hash.key, hash.value);
      }

      entry->clear();
      delete entry;
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
  std::vector<bucket *> storage;
  std::size_t count = 0, bucketCount = minimumBucketCount;
  float currentLoadFactor = 0;
  bool currentlyRehashing = false; // Lock for Insert in rehash
};
} // namespace Tourmaline::Containers
#endif
