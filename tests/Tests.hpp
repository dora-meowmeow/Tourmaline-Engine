/*
 * SPDX-FileCopyrightText: Dora "cat" <cat@thenight.club>
 * SPDX-License-Identifier: MPL-2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at http://mozilla.org/MPL/2.0/.
 */
#ifndef GUARD_TOURMALINE_TESTS_H
#define GUARD_TOURMALINE_TESTS_H

#include "Corrade/Containers/StringStlHash.h" // IWYU pragma: keep
#include "Corrade/TestSuite/Tester.h"

#define CORRADE_VERIFY_TYPE(x, y) CORRADE_VERIFY(typeid(x) == typeid(y))

class Tests : public Corrade::TestSuite::Tester {
public:
  Tests() {
    // Random
    addTests({&Tests::Random_SetSeed});
    addBenchmarks({&Tests::Random_Benchmark_GeneratingIntegers},
                  randomBatchCount);

    // ECS
    addBenchmarks(
        {&Tests::ECS_Benchmark_GoalCreate, &Tests::ECS_Benchmark_FetchAndStep},
        1);
    addBenchmarks({&Tests::ECS_Benchmark_Step}, stepBatchCount);
    addBenchmarks({&Tests::ECS_Benchmark_60Steps}, multiStepBatchCount);

    // Logging
    addTests({&Tests::Logging_ConsoleAndFileSync, &Tests::Logging_FileSwitching,
              &Tests::Logging_ErrorAndCritical});

    // Serialization
    addTests({&Tests::Serialization_CanSerialize,
              &Tests::Serialization_CanDeserialize});

    // BitArray
    addTests({&Tests::BitArray_SetBits, &Tests::BitArray_FlipBits,
              &Tests::BitArray_FindFirst, &Tests::BitArray_SetAll});

    // Hashlist
    addTests({&Tests::Hashlist_Insert, &Tests::Hashlist_Has,
              &Tests::Hashlist_Remove, &Tests::Hashlist_ListAndExtract,
              &Tests::Hashlist_Clear});

    // ChunkList
    addTests({&Tests::ChunkList_Insert, &Tests::ChunkList_Get,
              &Tests::ChunkList_Remove, &Tests::ChunkList_Iterator,
              &Tests::ChunkList_Clear});
    // ChunkList Benchmarking
    addBenchmarks({&Tests::ChunkList_Benchmark_IteratorNoGaps,
                   &Tests::ChunkList_Benchmark_NonIteratorNoGaps},
                  chunklistBatchCount);
    addTests({&Tests::ChunkList_Benchmark_HelperMakeGaps});
    addBenchmarks({&Tests::ChunkList_Benchmark_IteratorWithGaps,
                   &Tests::ChunkList_Benchmark_NonIteratorWithGaps},
                  chunklistBatchCount);
  }

  // Tourmaline::Systems::Random
  static constexpr uint32_t randomBatchCount = 10;
  void Random_SetSeed();
  void Random_Benchmark_GeneratingIntegers();

  // Tourmaline::Systems::ECS
  static constexpr uint32_t multiStepBatchCount = 5, stepBatchCount = 20;
  void ECS_Benchmark_GoalCreate();
  void ECS_Benchmark_FetchAndStep();
  void ECS_Benchmark_60Steps();
  void ECS_Benchmark_Step();

  // Tourmaline::Systems::Logging
  void Logging_ConsoleAndFileSync();
  void Logging_FileSwitching();
  void Logging_ErrorAndCritical();

  // Tourmaline::Systems::Serialization
  void Serialization_CanSerialize();
  void Serialization_CanDeserialize();

  // Tourmaline::Containers::BitArray
  void BitArray_SetBits();
  void BitArray_FlipBits();
  void BitArray_FindFirst();
  void BitArray_SetAll();

  // Tourmaline::Containers::Hashlist
  void Hashlist_Insert();
  void Hashlist_Has();
  void Hashlist_Remove();
  void Hashlist_ListAndExtract();
  void Hashlist_Clear();

  // Tourmaline::Containers::ChunkList
  void ChunkList_Insert();
  void ChunkList_Get();
  void ChunkList_Remove();
  void ChunkList_Iterator();
  void ChunkList_Clear();

  static constexpr uint32_t chunklistBatchCount = 20;
  void ChunkList_Benchmark_IteratorNoGaps();
  void ChunkList_Benchmark_NonIteratorNoGaps();
  void ChunkList_Benchmark_HelperMakeGaps();
  void ChunkList_Benchmark_IteratorWithGaps();
  void ChunkList_Benchmark_NonIteratorWithGaps();

  // Helpers
  void Helper_SwitchSTDOUT(bool fakeSTD);
};

#endif
