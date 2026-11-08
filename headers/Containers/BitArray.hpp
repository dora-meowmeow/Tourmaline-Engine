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

/**
 * @file
 * @brief An array of mutatable and queriable bits.
 */

namespace Tourmaline::Containers {
class BitArray {
public:
  /**
   * @brief Default constructor
   *
   * @param options See Tourmaline::Containers::BitArrayOptions
   */
  BitArray(BitArrayOptions options) : storedBitCount(options.BitCount) {
    // Count
    wholeCount = storedBitCount / sizeBitCount;
    remainderBits = storedBitCount % sizeBitCount;

    // Final variable mask
    arraySize = wholeCount;
    if (remainderBits > 0) {
      std::size_t discard = sizeBitCount - remainderBits;
      sizeRemainderMask = (sizeFullMask << discard) >> discard;
      ++arraySize;
    }

    array = std::make_unique<std::size_t[]>(arraySize);
    SetAll(options.SetAllBits);
  };

  /**
   * @brief Returns true/false based on the state of a given bit
   *
   * @param index Position of the bit you want to learn the state of.
   *
   * @return True if the bit equals 1, false if it equals 0.
   */
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

  /**
   * @brief Sets a given bit's state to 0 or 1
   *
   * @param index Position of the bit you want to set the state of.
   * @param value True sets the bit to 1, false sets the bit to 0.
   */
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

  /**
   * @brief Flips a given bit's state from 1 to 0 and vice versa
   *
   * @param index Position of the bit you want to flip the state of.
   */
  void FlipBit(std::size_t index) {
    Systems::Logging::Log("An out-of-bounds index was given! Throwing...",
                          "BitArray/FlipBit", Systems::Logging::Error,
                          index >= storedBitCount);
    std::size_t offsetMask = static_cast<std::size_t>(1)
                             << index % sizeBitCount;

    // Flip
    array[index / sizeBitCount] ^= offsetMask;
  }

  /**
   * @brief Finds the first bit that is set to either 1 or 0.
   *
   * @param value True if looking for the first bit that is set to 1,
   * false for 0.
   * @param start Where to begin the search from (inclusive).
   *
   * @return Position of the bit. If no bit is found with the requested value,
   * the function will return a value greater than or equal to storedBitCount.
   */
  std::size_t FindFirst(bool value, std::size_t start = 0) const {
    Systems::Logging::Log("An out-of-bounds index was given! Throwing...",
                          "BitArray/FindFirst", Systems::Logging::Error,
                          start >= storedBitCount);

    std::size_t index = start / sizeBitCount, skipBits = start % sizeBitCount;

    // TODO: Merge the logic of find 1 and find 0

    // Find next one
    if (value) {
      // Trying to pass the awkward placement
      if (skipBits) {
        std::size_t result = 0;
        if (array[index] != 0) {
          result = std::countr_zero(array[index] >> skipBits);
          if (result < (sizeBitCount - skipBits)) {
            return (index * sizeBitCount) + skipBits + result;
          }
        }
        ++index;
      }

      for (; index < arraySize; index++) {
        if (array[index] == 0) {
          continue;
        }
        return (index * sizeBitCount) + std::countr_zero(array[index]);
      }

      return storedBitCount;
    }

    // Find next zero
    bool isMiddle = index < wholeCount;
    std::size_t comparisonMask = isMiddle ? sizeFullMask : sizeRemainderMask;
    if (skipBits) {
      std::size_t result = 0;
      if (array[index] != comparisonMask) {
        result = std::countr_one(array[index] >> skipBits);
        if (result < (isMiddle ? sizeBitCount : remainderBits) - skipBits) {
          return (index * sizeBitCount) + skipBits + result;
        }
      }
      ++index;
    }

    for (; index < arraySize; index++) {
      comparisonMask = index < wholeCount ? sizeFullMask : sizeRemainderMask;
      if (array[index] == comparisonMask) {
        continue;
      }
      return (index * sizeBitCount) + std::countr_one(array[index]);
    }

    return storedBitCount;
  }

  /**
   * @brief Sets all the bits to 1 or 0.
   *
   * @param value True to set all the bits to 1, false to set all the bits to 0.
   */
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

  /**
   * @brief View of the stored bits.
   *
   * @return A span of the raw array.
   */
  std::span<std::size_t> ViewInternalArray() const noexcept {
    return {array.get(), arraySize};
  };

  /**
   * @brief Amount of bits stored by this array
   */
  std::size_t storedBitCount = 0;

  // TODO: add copy constructor and copy assign
private:
  constexpr static std::size_t sizeFullMask = SIZE_MAX,
                               sizeBitCount = sizeof(size_t) * CHAR_BIT;
  std::size_t wholeCount = 0, arraySize = 0, remainderBits = 0,
              sizeRemainderMask = 0;
  std::unique_ptr<std::size_t[]> array;
};
} // namespace Tourmaline::Containers
#endif
