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
template <Concepts::Hashable Key, typename Value, float maxLoadFactor = 0.75f,
          std::size_t baseBucketCount = 1024>
class Hashmap {
public:
  Hashmap() { storage.resize(baseBucketCount); }
  ~Hashmap() { Clear(); }

  Value &Insert(Key key, Value value) {
    std::size_t keyHash = std::hash<Key>{}(key),
                keyHashPosition = keyHash % storage.size();

    // Empty bucket
    if (storage[keyHashPosition] == nullptr) {
      storage[keyHashPosition] = new std::vector<hashStorage>;
      storage[keyHashPosition]->emplace_back(key, std::move(value), keyHash);
      return storage[keyHashPosition]->back().value;
    }

    // Throws
    Systems::Logging::Log("Trying to inserting same key twice! Throwing...",
                          "Hashmap", Systems::Logging::LogLevel::Error,
                          Has(key));

    storage[keyHashPosition]->emplace_back(key, std::move(value), keyHash);
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
  }

  bool Has(const Key &key) {
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
  }

private:
  struct hashStorage {
    Key key;
    Value value;
    std::size_t hash;
  };

  using bucket = std::vector<hashStorage>;
  std::vector<bucket *> storage;
};
} // namespace Tourmaline::Containers
#endif
