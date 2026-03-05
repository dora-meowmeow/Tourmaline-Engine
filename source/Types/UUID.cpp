/*
 * SPDX-FileCopyrightText: Dora "cat" <cat@thenight.club>
 * SPDX-License-Identifier: MPL-2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "../../headers/Types.hpp"

#include <charconv>
#include <cstdint>
#include <cstring>
#include <format>
#include <string>

using namespace Tourmaline::Type;
std::string UUID::asString() const {
  return std::format("{:016X}{:016X}", firstHalf, secondHalf);
}

bool UUID::operator==(const UUID &rhs) const {
  // Since size may be increased
  return firstHalf == rhs.firstHalf && secondHalf == rhs.secondHalf;
}

UUID::UUID(uint64_t firstHalf, uint64_t secondHalf)
    : firstHalf(firstHalf), secondHalf(secondHalf) {}

UUID::UUID(const std::string &uuid) {
  // We are assuming that it is a valid UUID, if not then somewhere else this
  // UUID should cause an error
  auto start = uuid.c_str(), half = start + 16,
       tail = half + 16; // Each UUID element is 16 characters padded

  std::from_chars(start, half, firstHalf, 16);
  std::from_chars(half, tail, secondHalf, 16);
}
