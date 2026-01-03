/*
 * SPDX-FileCopyrightText: Dora "cat" <cat@thenight.club>
 * SPDX-License-Identifier: MPL-2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "../Types.hpp"
#include <algorithm>
#include <format>
#include <memory>
#include <string>

using namespace Tourmaline::Type;
std::string UUID::asString() const {
  return std::format("{:016X}{:016X}", data[0], data[1]);
}

UUID::UUID(UUID &&uuid) noexcept { data.swap(uuid.data); }

UUID::UUID(uint64_t firstHalf, uint64_t secondHalf) {
  data[0] = firstHalf;
  data[1] = secondHalf;
}

UUID::UUID(const std::string &uuid) {
  // We are assuming that it is a valid UUID, if not then somewhere else this
  // UUID should cause an error
  std::copy(uuid.begin(), uuid.begin() + 32, data.get());
}
