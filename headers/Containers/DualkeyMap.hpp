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
#include "Hashing.hpp"

#include <cstddef>
#include <cstdint>
#include <vector>

namespace Tourmaline::Containers {
template <Hashable AKey, Hashable BKey, typename Value,
          uint64_t baseReservation = 1024,
          float reservationGrowthExponent = 1.5>
class DualkeyMap {
  DualkeyMap() {
    ValueList.reserve(baseReservation);
    HashList.reserve(baseReservation);
  }

  ~DualkeyMap() {
    // I'm sure there is a better way to do this
    for (auto value : ValueList) {
      delete value;
    }

    for (auto hash : HashList) {
      delete hash.Apointer;
      delete hash.Bpointer;
    }
  }

  // No copying, No moving. Moving may be valid in the future.
  // However as of now it is not a wise way to use this map.
  DualkeyMap(const DualkeyMap &) = delete;
  DualkeyMap(DualkeyMap &&) = delete;
  DualkeyMap &operator=(const DualkeyMap &) = delete;
  DualkeyMap &operator=(DualkeyMap &&) = delete;

private:
  struct DualkeyHash {
    DualkeyHash(std::size_t AHash, AKey *APointer, std::size_t BHash,
                BKey *BPointer)
        : AKeyHash(AHash), APointer(APointer), BKeyHash(BHash),
          BPointer(BPointer) {}
    std::size_t AKeyHash = 0;
    std::size_t BKeyHash = 0;
    AKey *APointer;
    BKey *BPointer;
  };
  std::vector<Value *> ValueList;
  std::vector<DualkeyHash> HashList;
};
} // namespace Tourmaline::Containers
#endif
