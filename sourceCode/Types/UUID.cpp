/*
 * SPDX-FileCopyrightText: Dora "cat" <cat@thenight.club>
 * SPDX-License-Identifier: MPL-2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "../Types.hpp"
#include <charconv>
#include <cstdint>
#include <cstring>
#include <format>
#include <memory>
#include <string>

using namespace Tourmaline::Type;
std::string UUID::asString() const {
  return std::format("{:016X}{:016X}", data[0], data[1]);
}

UUID::UUID(const UUID &uuid) {
  std::memcpy(data.get(), uuid.data.get(), UUID::ByteLength);
}

UUID &UUID::operator=(const UUID &uuid) {
  if (this != &uuid) [[likely]] {
    std::memcpy(data.get(), uuid.data.get(), UUID::ByteLength);
  }

  return *this;
}

UUID &UUID::operator=(UUID &&uuid) {
  if (this != &uuid) [[likely]] {
    data.swap(uuid.data);
  }

  return *this;
}

UUID::UUID(UUID &&uuid) noexcept { data.swap(uuid.data); }

UUID::UUID(uint64_t firstHalf, uint64_t secondHalf) {
  data[0] = firstHalf;
  data[1] = secondHalf;
}

UUID::UUID(const std::string &uuid) {
  // We are assuming that it is a valid UUID, if not then somewhere else this
  // UUID should cause an error
  auto start = uuid.c_str(), half = start + ByteLength,
       tail = half + ByteLength; // Each UUID element is 16 characters padded

  std::from_chars(start, half, data[0], 16);
  std::from_chars(half, tail, data[1], 16);
}
