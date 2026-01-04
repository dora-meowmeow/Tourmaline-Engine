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
  constexpr static uint8_t BitLength = 128;
  constexpr static uint8_t QWORDLength = BitLength / 64;
  constexpr static uint8_t ByteLength = BitLength / 8;

  [[nodiscard]]
  std::string asString() const;

  UUID(uint64_t firstHalf, uint64_t secondHalf);
  UUID(const std::string &uuid);

  UUID(const UUID &uuid);
  UUID(UUID &&uuid) noexcept;
  UUID &operator=(const UUID &uuid);
  UUID &operator=(UUID &&uuid);
  ~UUID() = default;

private:
  std::unique_ptr<uint64_t[]> data = std::make_unique<uint64_t[]>(QWORDLength);
};
} // namespace Tourmaline::Type
#endif
