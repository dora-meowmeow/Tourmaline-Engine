/*
 * SPDX-FileCopyrightText: Dora "cat" <cat@thenight.club>
 * SPDX-License-Identifier: MPL-2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at http://mozilla.org/MPL/2.0/.
 */
#ifndef GUARD_TOURMALINE_BITARRAY_H
#define GUARD_TOURMALINE_BITARRAY_H

#include "../Systems/Logging.hpp"
#include "ContainerOptions.hpp"

#include <bit>
#include <climits>
#include <memory>
#include <span>

namespace Tourmaline::Containers {
class BitArray {
public:
  BitArray(BitArrayOptions options) : storedBitCount(options.BitCount) {
    // Count
    endBitPosition = storedBitCount;
    wholeCount = storedBitCount / sizeBitCount;
    remainderBits = storedBitCount % sizeBitCount;

    // Final variable mask
    arraySize = wholeCount;
    remainderExists = remainderBits > 0;
    if (remainderExists) {
      discard = sizeBitCount - remainderBits;
      sizeRemainderMask = (sizeFullMask << discard) >> discard;
      ++arraySize;
    }

    array = std::make_unique<std::size_t[]>(arraySize);
    SetAll(options.SetAllBits);
  };

  bool GetBit(std::size_t index) const {
    // Guarding
    Systems::Logging::Log("An out-of-bounds index was given! Throwing...",
                          "BitArray/GetBit", Systems::Logging::Error,
                          index >= storedBitCount);

    // Flipping so 0 points to end and last bit is sizeBitCount
    std::size_t container = array[index / sizeBitCount],
                offset = index % sizeBitCount;

    return (container >> offset) & 1;
  }

  void SetBit(std::size_t index, bool value) {
    Systems::Logging::Log("An out-of-bounds index was given! Throwing...",
                          "BitArray/SetBit", Systems::Logging::Error,
                          index >= storedBitCount);
    std::size_t offsetMask = static_cast<std::size_t>(1)
                             << index % sizeBitCount;

    // Switching on
    if (value) {
      array[index / sizeBitCount] |= offsetMask;
      return;
    }
    // Switching off
    array[index / sizeBitCount] &= ~offsetMask;
    return;
  }

  void FlipBit(std::size_t index) {
    Systems::Logging::Log("An out-of-bounds index was given! Throwing...",
                          "BitArray/FlipBit", Systems::Logging::Error,
                          index >= storedBitCount);
    std::size_t offsetMask = static_cast<std::size_t>(1)
                             << index % sizeBitCount;

    // Flip
    array[index / sizeBitCount] ^= offsetMask;
  }

  std::size_t FindFirst(bool value) const noexcept {
    // Find next one
    if (value) {
      for (std::size_t index = 0; index < arraySize; index++) {
        if (array[index] == 0) {
          continue;
        }
        return (index * sizeBitCount) + std::countr_zero(array[index]);
      }

      return endBitPosition;
    }

    // Find next zero
    std::size_t comparisonMask;
    for (std::size_t index = 0; index < arraySize; index++) {
      comparisonMask = index < wholeCount ? sizeFullMask : sizeRemainderMask;
      if (array[index] == comparisonMask) {
        continue;
      }
      return (index * sizeBitCount) + std::countr_one(array[index]);
    }

    return endBitPosition;
  }

  void SetAll(bool value = false) noexcept {
    if (value) {
      for (std::size_t index = 0; index < arraySize; index++) {
        if (index < wholeCount) {
          array[index] = sizeFullMask;
          continue;
        }
        array[index] = sizeRemainderMask;
      }
      return;
    }

    // Clearing is cheaper
    for (std::size_t index = 0; index < arraySize; index++) {
      array[index] = 0;
    }
  }

  std::span<const std::size_t> ViewInternalArray() const noexcept {
    return {array.get(), arraySize};
  };

  std::size_t endBitPosition = 0;

  // TODO: add copy constructor and copy assign
private:
  constexpr static std::size_t sizeFullMask = SIZE_MAX,
                               sizeBitCount = sizeof(size_t) * CHAR_BIT;
  std::size_t storedBitCount = 0, remainderBits = 0, wholeCount = 0,
              discard = 0, arraySize = 0, sizeRemainderMask = 0;
  bool remainderExists = false;
  std::unique_ptr<std::size_t[]> array;
};
} // namespace Tourmaline::Containers
#endif
