/*
 * SPDX-FileCopyrightText: Dora "cat" <cat@thenight.club>
 * SPDX-License-Identifier: MPL-2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at http://mozilla.org/MPL/2.0/.
 */
#ifndef GUARD_TOURMALINE_CONTAINEROPTIONS_H
#define GUARD_TOURMALINE_CONTAINEROPTIONS_H

#include <cstddef>
#include <cstdint>

/**
 * @file
 * @brief Configuration options for containers.
 */

namespace Tourmaline::Containers {

/**
 * @brief Configuration for Hashlist and Hashmap.
 *
 * This is used by Tourmaline::Containers::Hashlist and
 * Tourmaline::Containers::Hashmap to determine how to allocate
 * and grow the containers.
 */
struct HashContainerOptions {
  /**
   * @brief Maximum load factor before calling a rehash.
   *
   * When the load factor hits at least this percentage,
   * a rehash will happen to expand the container.
   */
  float loadFactor = 0.75f;

  /**
   * @brief Minimum load factor percentage to rehash.
   * This will be used for shrinking the container.
   */
  float minimizeFactor = 0.20f;

  /**
   * @brief Higher values mean leaning more towards minimizeFactor when
   * recalculating a load factor.
   *
   * When reallocating, the newly reallocated container will calculate
   * the desired load factor with the following formula:
   *
   * newLoadFactor = (loadFactor + minimizeFactor) / leaningFactor
   *
   * This value should generally be set to make newLoadFactor be between
   * loadFactor and minimizeFactor. If your container needs to grow gradually,
   * set it to a lower value. Otherwise set it to a higher value.
   */
  float leaningFactor = 2.5f;

  /**
   * @brief Minimum amount of buckets.
   *
   * Minimum amount of buckets to hold hashes. A bigger value
   * hypothetically means less hash collisions, so the insertion and
   * fetching speed is faster. However, more buckets means more memory is used.
   */
  std::size_t minimumBucketCount = 256;

  /**
   * @brief Per bucket reserved space.
   *
   * Each bucket is a vector under the hood. In order to avoid unnecessary
   * reallocations, the bucket will reserve this much space for entries.
   */
  std::size_t reservedBucketSpace = 4;
};

/**
 * @brief Configuration for Hashlist and Hashmap.
 *
 * This is used by Tourmaline::Containers::Hashlist and
 * Tourmaline::Containers::Hashmap to determine how to allocate
 * and grow the containers.
 */
struct DualKeyMapOptions {
  /// @brief Base amount of entries to reserve.
  std::uint64_t baseReservation = 2048;
};

/**
 * @brief Configuration for ChunkList.
 *
 * This is used by Tourmaline::Containers::ChunkList to determine how the chunks
 * inside it should be.
 */
struct ChunkListOptions {
  /**
   * @brief How many slots should each chunk have.
   *
   * @note The default value is picked after several tests showed it to be most
   * optimal. We still decided to allow chunk sizes to be variable for
   * flexibility. However we do suggest staying in 2000 to 3000 slots (intervals
   * of 64 is even better) per chunk range for most use cases.
   *
   * @warning This value cannot be 0!
   */
  std::size_t ChunkCapacity = 2048;
};

/**
 * @brief Configuration for BitArray.
 *
 * This is used by Tourmaline::Containers::BitArray to determine
 * how many bits are needed to allocate and what to set each bit to.
 */
struct BitArrayOptions {
  /**
   * @brief Amount of bits to allocate.
   *
   * @note While you can set this to any value you want. Any value that is not a
   * multiple of your cpu's address size will be not used but allocated.
   * For example: if you are on a 32-bit system and want 37 bits, the BitArray
   * will need 64 bits regardless. So the 27 bits will be unused. Whilst if
   * you had a BitArray of 64 bits it would still use 64 bits.
   */
  std::size_t BitCount;

  /**
   * @brief What every bit should be set to
   *
   * Setting all the bits to 0 is a tiny bit faster than setting them all to 1.
   */
  bool SetAllBits = false;
};
} // namespace Tourmaline::Containers
#endif
