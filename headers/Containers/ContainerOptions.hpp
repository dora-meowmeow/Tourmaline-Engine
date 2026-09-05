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
   * @f(newLoadFactor = (loadFactor + minimizeFactor) / leaningFactor@f)
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
  size_t minimumBucketCount = 256;

  /**
   * @brief Per bucket reserved space.
   *
   * Each bucket is a vector under the hood. In order to avoid unnecessary
   * reallocations, the bucket will reserve this much space for entries.
   */
  size_t reservedBucketSpace = 4;
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
  uint64_t baseReservation = 2048;
};

struct BloomFilterOptions {
  uint64_t bloomCount = 8;
  uint64_t bytesPerBloom = 256;
};
} // namespace Tourmaline::Containers
#endif
