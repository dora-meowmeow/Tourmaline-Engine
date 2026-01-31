/*
 * SPDX-FileCopyrightText: Dora "cat" <cat@thenight.club>
 * SPDX-License-Identifier: MPL-2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef GUARD_TOURMALINE_TYPES_H
#define GUARD_TOURMALINE_TYPES_H

#include "TourmalineExternal/random/xoshiro.h"
#include <cstdint>
#include <functional>
#include <string>

namespace Tourmaline::Type {
class UUID {
public:
  [[nodiscard]]
  std::string asString() const;
  bool operator==(const UUID &rhs) const;

  UUID(uint64_t firstHalf, uint64_t secondHalf);
  UUID(const std::string &uuid);

private:
  uint64_t firstHalf = 0, secondHalf = 0;
  friend struct std::hash<Tourmaline::Type::UUID>;
};
} // namespace Tourmaline::Type

namespace std {
template <> struct hash<Tourmaline::Type::UUID> {
  size_t operator()(const Tourmaline::Type::UUID &uuid) const noexcept {
    uint64_t hash = Xoshiro::splitmix64(uuid.firstHalf);
    hash += uuid.secondHalf;
    return Xoshiro::splitmix64(hash);
  }
};

} // namespace std

#endif
