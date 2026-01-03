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
#include <cstdint>
#include <memory>
namespace Tourmaline::Type {
struct UUID {
  [[nodiscard]]
  std::string asString() const;

  UUID(uint64_t firstHalf, uint64_t secondHalf);
  UUID(const std::string &uuid);
  UUID(UUID &&uuid) noexcept;

private:
  std::unique_ptr<uint64_t[]> data = std::make_unique<uint64_t[]>(2);
};
} // namespace Tourmaline::Type
#endif
