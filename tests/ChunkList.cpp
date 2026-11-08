/*
 * SPDX-FileCopyrightText: Dora "cat" <cat@thenight.club>
 * SPDX-License-Identifier: MPL-2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "Containers/ChunkList.hpp"
#include "Containers/Hashlist.hpp"
#include "Corrade/TestSuite/Tester.h"

#include "Systems/Random.hpp"
#include "Tests.hpp"

#include <cstdint>

using namespace Corrade;
using namespace Tourmaline::Systems;

constexpr std::size_t goal = 10'000, chunks = 5, chunkCapacity = goal / chunks,
                      removeEntryCount = goal / 100;
constexpr std::size_t benchmarkRemoveEntryCount = removeEntryCount * 10;
Tourmaline::Containers::ChunkList<uint32_t, {chunkCapacity}> chunkList;

void Tests::ChunkList_Insert() {
  for (uint32_t x = 0; x < goal; x++) {
    chunkList.Insert(Random::Generate(1024));
  }

  CORRADE_VERIFY(chunkList.CountEntries() == goal &&
                 chunkList.CountChunks() == chunks);
}

void Tests::ChunkList_Get() {
  // I mean this should overflow to zero sooo :shrug:
  std::size_t chunkCount = -1, slotPosition = 0;
  for (uint32_t index = 0; index < goal; index++) {
    slotPosition = index % chunkCapacity;
    if (slotPosition == 0) {
      ++chunkCount;
    }

    try {
      if (chunkList.Get({chunkCount, slotPosition}) > 1024) {
        CORRADE_FAIL_IF(
            true,
            "The value was not set to given range at ChunkList_Insert! Chunk"
                << chunkCount << "slot" << slotPosition);
      }
    } catch (...) {
      CORRADE_FAIL_IF(
          true,
          "Failed to fetch value even though it should've been there! Chunk"
              << chunkCount << "slot" << slotPosition);
    }
  }
  CORRADE_VERIFY(true);
}

void Tests::ChunkList_Remove() {
  Tourmaline::Containers::Hashlist<uint32_t> alreadyDid;
  for (uint32_t index = 0; index < removeEntryCount; index++) {
    uint32_t randomEntry = Random::Generate(goal - 1);
    while (alreadyDid.Has(randomEntry)) {
      randomEntry = Random::Generate(goal - 1);
    }
    alreadyDid.Insert(randomEntry);
    std::size_t slot = randomEntry % chunkCapacity,
                chunk = randomEntry / chunkCapacity;
    CORRADE_FAIL_IF(!chunkList.Remove({chunk, slot}),
                    "Failed to remove entry in bound Chunk" << chunk << "slot"
                                                            << slot);
  }
  CORRADE_COMPARE(chunkList.CountEntries(), goal - removeEntryCount);
}

void Tests::ChunkList_Iterator() {
  // Doing total so the compiler doesn't remove this with -O3 or -O4
  uint32_t count = 0, total = 0;
  for (uint32_t val : chunkList) {
    total += val;
    ++count;
  }

  CORRADE_FAIL_IF(
      total == 0,
      "Variables was not added to the total. Failed to copy/fetch?");

  CORRADE_FAIL_IF(
      count != goal - removeEntryCount,
      "Count was not equal to expected value. Some entries were skipped!"
          << count << goal << removeEntryCount << goal - removeEntryCount);
  CORRADE_VERIFY(true);
}

void Tests::ChunkList_Clear() {
  chunkList.Clear();
  CORRADE_FAIL_IF(chunkList.CountEntries() != 0,
                  "CountEntries should be 0 but wasn't!");
  CORRADE_FAIL_IF(chunkList.CountChunks() != 0,
                  "CountChunks should be 0 but wasn't!");

  // This should be empty
  uint32_t total = 0;
  for (uint32_t val : chunkList) {
    total += val;
  }
  CORRADE_FAIL_IF(total != 0,
                  "Iterator ran even thought the chunklist was cleared out!");

  // Check reusability
  for (uint32_t x = 0; x < goal; x++) {
    chunkList.Insert(Random::Generate(1024));
  }

  CORRADE_VERIFY(chunkList.CountEntries() == goal &&
                 chunkList.CountChunks() == chunks);
#ifdef TOURMALINE_WITH_BENCHMARKS
  Logging::LogFormatted("Entry count is {} for benchmark runs.", "Benchmark",
                        Logging::Debug, chunkList.CountEntries());
#endif
}

void Tests::ChunkList_Benchmark_IteratorNoGaps() {
  CORRADE_BENCHMARK(chunklistBatchCount) {
    for (uint32_t &val : chunkList) {
      val *= 2;
    }
  }
}

void Tests::ChunkList_Benchmark_NonIteratorNoGaps() {
  CORRADE_BENCHMARK(chunklistBatchCount) {
    for (uint32_t index = 0; index < removeEntryCount; index++) {
      Tourmaline::Containers::ChunkListRecord record{index / chunkCapacity,
                                                     index % chunkCapacity};
      chunkList.Get(record) *= 2;
    }
  }
}

void Tests::ChunkList_Benchmark_HelperMakeGaps() {
  // Making gaps here for the noniterator test as well!
  Tourmaline::Containers::Hashlist<uint32_t> alreadyDid;
  for (uint32_t index = 0; index < benchmarkRemoveEntryCount; index++) {
    uint32_t randomEntry = Random::Generate(goal - 1);
    while (alreadyDid.Has(randomEntry)) {
      randomEntry = Random::Generate(goal - 1);
    }
    alreadyDid.Insert(randomEntry);
    std::size_t slot = randomEntry % chunkCapacity,
                chunk = randomEntry / chunkCapacity;
    chunkList.Remove({chunk, slot});
  }

  Logging::LogFormatted(
      "Removed {} entries randomly. New entry count is {} for benchmark runs.",
      "Benchmark", Logging::Debug, benchmarkRemoveEntryCount,
      chunkList.CountEntries());

  CORRADE_VERIFY(true);
}

void Tests::ChunkList_Benchmark_IteratorWithGaps() {
  // Actual test
  CORRADE_BENCHMARK(chunklistBatchCount) {
    for (uint32_t &val : chunkList) {
      val *= 2;
    }
  }
}

void Tests::ChunkList_Benchmark_NonIteratorWithGaps() {
  CORRADE_BENCHMARK(chunklistBatchCount) {
    for (uint32_t index = 0; index < removeEntryCount; index++) {
      Tourmaline::Containers::ChunkListRecord record{index / chunkCapacity,
                                                     index % chunkCapacity};
      if (chunkList.Has(record)) {
        chunkList.Get(record) *= 2;
      }
    }
  }
}
