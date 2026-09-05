/*
 * SPDX-FileCopyrightText: Dora "cat" <cat@thenight.club>
 * SPDX-License-Identifier: MPL-2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef GUARD_TOURMALINE_BLOOMFILTER_H
#define GUARD_TOURMALINE_BLOOMFILTER_H

#include "../Concepts.hpp"
#include "../Systems/Random.hpp"
#include "ContainerOptions.hpp"

#include <cstdint>
#include <utility>
#include <vector>

namespace Tourmaline::Containers {

class BloomFilter {
public:
  BloomFilter(BloomFilterOptions options = {}) : Options(options) {
    // we can do odd/even to only populate half of the entries
    // lowering saturation. From my tests diving to 3rd or 4ths is best:
    //
    // so per entry there should be 4 bits flipped
    //
    // We can do the storage as options.bytesPerBloom
    // this does mean that need to store stuff in uint8 arrays
    // instead of uint64_t
    //
    // these two techniques should severly lower the saturation
    //
    // additionally the bloom should be able to be grown. At the end of the day
    // we always known how much exactly memory is needed per construction so we
    // can use that to our benefits. We can allocate single time per growth.
    //
    bloomValues.reserve(options.bloomCount);

    for (uint64_t x = 0; x < options.bloomCount; x++) {
      bloomValues.push_back({Systems::Random::Generate(1.0, 0.00000001), 0});
    }
  }

  template <Concepts::Hashable Value> void Insert(const Value &value) {
    // Getting only bottom half due to the fact that we want our hash to fit
    // into a double (uint64_t at most is 64 bits while double is only 52 for
    // fraction)
    uint32_t hash = std::hash<Value>{}(value);
    for (auto &bloomValue : bloomValues) {
      uint64_t index = 1ULL
                       << (static_cast<uint64_t>(hash * bloomValue.first) % 64);
      uint64_t &storage = bloomValue.second;

      // If the same value already exists
      if (storage & index) {
        continue;
      }
      storage += index;
    }
    hashStorage.push_back(hash);
  }

  template <Concepts::Hashable Value> bool Has(const Value &value) {
    // See Insert on why this is uint32_t
    uint32_t hash = std::hash<Value>{}(value);

    for (const auto &bloomValue : bloomValues) {
      uint64_t index = 1ULL
                       << (static_cast<uint64_t>(hash * bloomValue.first) % 64);
      const uint64_t &storage = bloomValue.second;

      // If the same value already exists
      if (storage & index) {
        continue;
      }
      return false;
    }

    return true;
  }

  template <Concepts::Hashable Value> bool Remove(const Value &value) {

    // This is a bloom check and it is possible for this to be false positive,
    // try to remvoe the hash first and see if it returns something (erase_if)
    //
    // if there is an entry then you can be certain
    //
    // Additionally do repopulation as a seperate thing (maybe automated)
    // If there is x% of entries removed or whatever
    if (!Has(value)) {
      return false;
    }

    // See Insert on why this is uint32_t
    uint32_t hash = std::hash<Value>{}(value);

    // No other way :(
    std::erase_if(hashStorage, [&](const uint64_t &storedHash) {
      if (storedHash == hash) {
        return true;
      }

      Insert(storedHash);
      return false;
    });

    return true;
  }

  void Clear() {
    for (auto &bloomValue : bloomValues) {
      bloomValue.second = 0;
    }
    hashStorage.clear();
  }

  size_t Count() { return hashStorage.size(); }

private:
  BloomFilterOptions Options;
  std::vector<std::pair<double, uint64_t>> bloomValues;

  // A function to extract this would be good
  std::vector<uint64_t> hashStorage;
};

}; // namespace Tourmaline::Containers
#endif
