/*
 * SPDX-FileCopyrightText: Dora "cat" <cat@thenight.club>
 * SPDX-License-Identifier: MPL-2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at http://mozilla.org/MPL/2.0/.
 */
#ifndef GUARD_TOURMALINE_DUALKEYMAP_H
#define GUARD_TOURMALINE_DUALKEYMAP_H
#include <cstddef>
#include <vector>

namespace Tourmaline::Containers {
template <typename A, typename B, typename Value> class DualkeyMap {

private:
  struct DualkeyHash {
    DualkeyHash(std::size_t AHash, A *APointer, std::size_t BHash, B *BPointer)
        : AKeyHash(AHash), APointer(APointer), BKeyHash(BHash),
          BPointer(BPointer) {}
    std::size_t AKeyHash = 0;
    std::size_t BKeyHash = 0;
    A *APointer;
    B *BPointer;
  };
  std::vector<Value *> ValueList;
  std::vector<DualkeyHash> HashList{};
};
} // namespace Tourmaline::Containers
#endif
