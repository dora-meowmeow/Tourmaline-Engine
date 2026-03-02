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

namespace Tourmaline::Containers {
struct HashmapOptions {
  float loadFactor = 0.75f;
  float minimizeFactor = 0.20f;
  float leaningFactor = 2.5f;
  std::size_t minimumBucketCount = 256;
  std::size_t reservedBucketSpace = 4;
};

struct DualKeyMapOptions {
  std::uint64_t baseReservation = 2048;
};
} // namespace Tourmaline::Containers
#endif
