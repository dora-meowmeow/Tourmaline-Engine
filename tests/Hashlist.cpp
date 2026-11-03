/*
 * SPDX-FileCopyrightText: Dora "cat" <cat@thenight.club>
 * SPDX-License-Identifier: MPL-2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at http://mozilla.org/MPL/2.0/.
 */
#include "Corrade/Containers/String.h"
#include "Corrade/TestSuite/Tester.h"

#include "Containers/Hashlist.hpp"
#include "Tests.hpp"

#include <array>

using namespace Tourmaline::Systems;
using namespace Corrade;

// Dummy data
// Duplicates are knowingly added
constexpr std::size_t randomDataCount = 20;
constexpr std::size_t uniqueCount =
    20 - 3; // This is 1 original and 3 duplicates

Tourmaline::Containers::Hashlist<Corrade::Containers::String> hashlist;
std::array<Containers::String, randomDataCount> randomData{
    "Magnum Graphics is awesome!",
    "The shark plushies are filling my room! D:",
    "Viruses do not exist. Illness is simply your body punishing you for "
    "what you have done wrong.",
    "Wait a minute you have merch? Yeah, I get a 650$ royalty check every "
    "month.",
    "That's a stupid saying.",
    "How many \"abo-digitals\" do you see modeling ?",
    "Noxy can't speel",
    "Magnum Graphics is awesome!",
    "Magnum Graphics is awesome!",
    "caly: I don't care how long your arms are, you can't carry your own "
    "coffin",
    "Are you intentionally dense or something? HHHHUUUUHHHHH.",
    "coopi: Save me a spot on the moon, choom.",
    "Lovesick, cannibal! I bet I'll be better than ever when I get to run "
    "away with all of my mistakes!",
    "Dora <3 Sara",
    "Memory safety is programmer's responsibility.",
    "Bingles threw my family into the pit :(",
    "arniteur: Personal RAM ownership detected, reporting to Sam Altman",
    "Jerma how did you do the 100 kick football challenge in first try??",
    "I've been up for three days, everything is haunted. Everybody's evil "
    "and there's bugs inside the carpet.",
    "Magnum Graphics is awesome!"};

void Tests::Hashlist_Insert() {
  bool firstTime = true;
  Helper_SwitchSTDOUT(true);
  for (const Containers::String &str : randomData) {
    if (str == "Magnum Graphics is awesome!") {
      // No error
      if (firstTime) {
        hashlist.Insert(str);
        firstTime = false;
        continue;
      }

      // Should throw!!
      try {
        hashlist.Insert(str);
        Helper_SwitchSTDOUT(false);
        CORRADE_FAIL_IF(true, "Duplicate entry did not throw!");
        return;
      } catch (...) {
        continue;
      }
      continue;
    }
    hashlist.Insert(str);
  }
  Helper_SwitchSTDOUT(false);
  CORRADE_COMPARE(hashlist.Count(), uniqueCount);
}

void Tests::Hashlist_Has() {
  for (const Containers::String &str : randomData) {
    CORRADE_FAIL_IF(!hashlist.Has(str),
                    "An entry that should be in the hashlist is missing \""
                        << str << "\".");
  }
  CORRADE_VERIFY(true);
}

void Tests::Hashlist_Remove() {
  std::size_t count = 0;
  for (const Containers::String &str : randomData) {
    if (count++ % 3 == 0) {
      CORRADE_FAIL_IF(!hashlist.Remove(str),
                      "Failed to remove \"" << str << "\".");
    }
  }
  CORRADE_VERIFY(hashlist.Count() < uniqueCount);
}

void Tests::Hashlist_ListAndExtract() {
  for (const Containers::String &str : hashlist.ListAllEntries()) {
    CORRADE_FAIL_IF(!hashlist.Has(str),
                    "An entry that should be in the hashlist is missing \""
                        << str << "\".");
  }

  auto data = hashlist.ExtractAllEntries();
  CORRADE_FAIL_IF(hashlist.Count() != 0,
                  "The hashlist should've been empty after extract.");

  for (const Containers::String &str : data) {
    try {
      hashlist.Insert(str);
    } catch (...) {
      CORRADE_FAIL_IF(true,
                      "Failed to insert \"" << str << "\" into the hashlist.");
    }
  }

  CORRADE_COMPARE(hashlist.Count(), data.size());
}

void Tests::Hashlist_Clear() {
  hashlist.Clear();
  CORRADE_FAIL_IF(hashlist.Count() != 0,
                  "The hashlist should've been empty after clear.");
  Hashlist_Insert();
}
