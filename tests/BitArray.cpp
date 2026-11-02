/*
 * SPDX-FileCopyrightText: Dora "cat" <cat@thenight.club>
 * SPDX-License-Identifier: MPL-2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at http://mozilla.org/MPL/2.0/.
 */
#include "Containers/BitArray.hpp"
#include "Corrade/TestSuite/Tester.h"
#include "Systems/Random.hpp"
#include "Tests.hpp"
#include <cstddef>

using namespace Tourmaline::Systems;

void Tests::BitArray_SetBits() {
  for (uint8_t index = 0; index < 20; index++) {
    bitArray.SetBit(Random::Generate(91, 0), 1);
  }

  // Check if anything was switched
  for (std::size_t data : bitArray.ViewInternalArray()) {
    if (data != 0) {
      CORRADE_VERIFY(true);
      return;
    }
  }
  CORRADE_FAIL_IF(true, "None of the bits were set to 0!");
}

void Tests::BitArray_FlipBits() {
  for (uint8_t index = 0; index < 20; index++) {
    std::size_t position = Random::Generate(91, 0);
    bool oldValue = bitArray.GetBit(position);
    bitArray.FlipBit(position);
    CORRADE_FAIL_IF(oldValue == bitArray.GetBit(position),
                    "A bit wasn't flipped!");
  }
  CORRADE_VERIFY(true);
}

void Tests::BitArray_FindFirst() {
  bool noneFoundCase = false;
  for (uint8_t index = 0; index < 20; index++) {
    bool lookFor = Random::Generate(1, 0);
    std::size_t position = bitArray.FindFirst(lookFor);

    // No bit with that value was found
    if (position >= bitArray.storedBitCount) {
      if (!noneFoundCase) {
        CORRADE_INFO(
            "FindFirst returned an out of range value as expected since there "
            "is no bit that is set to what is being looked for.");
        noneFoundCase = true;
      }
      continue;
    }
    CORRADE_FAIL_IF(bitArray.GetBit(position) != lookFor,
                    "FindFirst found invalid value.");
    bitArray.FlipBit(position);
  }
  CORRADE_VERIFY(true);
}

void Tests::BitArray_SetAll() {
  bitArray.SetAll(false);
  CORRADE_FAIL_IF(bitArray.FindFirst(true) < bitArray.storedBitCount,
                  "Failed to set all bits to 0!");
  bitArray.SetAll(true);
  CORRADE_FAIL_IF(bitArray.FindFirst(false) < bitArray.storedBitCount,
                  "Failed to set all bits to 1!");

  CORRADE_VERIFY(true);
}
