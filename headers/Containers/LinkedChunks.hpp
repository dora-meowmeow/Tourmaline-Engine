/*
 * SPDX-FileCopyrightText: Dora "cat" <cat@thenight.club>
 * SPDX-License-Identifier: MPL-2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at http://mozilla.org/MPL/2.0/.
 */
#ifndef GUARD_TOURMALINE_LINKEDCHUNKS_H
#define GUARD_TOURMALINE_LINKEDCHUNKS_H
#include "../Systems/Logging.hpp"
#include "ContainerOptions.hpp"

#include "Corrade/Containers/BitArray.h"
#include "Corrade/Tags.h"

#include <climits>
#include <cstddef>

#include <array>
#include <optional>
#include <vector>

/**
 * @file
 * @brief Pre-defined sized arrays linked to each other. DO NOT USE!
 */

namespace Tourmaline::Containers {
template <typename EntryType, LinkedChunksOptions options = {}>
class LinkedChunks {
public:
  // I am not a big fan of the name "EntryPosition" might renamed to
  // Slot or SlotId or SlotPosition
  struct Record {
    std::size_t ChunkId = 0, EntryPosition = 0;
  };

  LinkedChunks() {
    Systems::Logging::Log("LinkedChunksOptions::ChunkCapacity cannot be 0 or "
                          "less! Terminating...",
                          "LinkedChunks", Systems::Logging::Critical,
                          options.ChunkCapacity == 0);
    chunks.push_back(new Chunk{});
  }
  ~LinkedChunks() { Clear(); }

  // No copying since the data is owned by instance of the said class
  // Deleting to mitigate shallow copying
  // Moving is fine
  LinkedChunks(const LinkedChunks &) = delete;
  LinkedChunks &operator=(const LinkedChunks &) = delete;
  LinkedChunks(LinkedChunks &&linkedChunk) noexcept
      : chunks(std::move(linkedChunk.chunks)),
        entryCount(linkedChunk.entryCount) {
    linkedChunk.entryCount = 0;
  }
  LinkedChunks &operator=(LinkedChunks &&rightHandSide) noexcept {
    if (this == &rightHandSide) {
      return *this;
    }
    Clear();
    this->entryCount = rightHandSide.entryCount;
    chunks = std::move(rightHandSide.chunks);
    rightHandSide.entryCount = 0;

    return *this;
  }

  Record Insert(const EntryType &entry) noexcept {
    Record chunkPair = getChunkWithEmptySlot();
    Chunk *chunk = chunks[chunkPair.ChunkId];
    std::size_t slot = chunkPair.EntryPosition;

    chunk->slots[slot] = entry;
    chunk->isSlotTaken.set(slot, true);
    ++entryCount;

    return chunkPair;
  }

  EntryType &Get(const Record &record) {
    Systems::Logging::Log(
        "LinkedChunks was given a record that was out of bounds! Throwing...",
        "LinkedChunks", Systems::Logging::Error, !isRecordValid(record));

    // Not using at since we already did bounds check above
    Entry &entry = chunks[record.ChunkId]->slots[record.EntryPosition];
    if (!entry.has_value()) {
      Systems::Logging::LogFormatted(
          "There is no valid data at chunk {} slot {}! Throwing...",
          "LinkedChunks", Systems::Logging::Error, record.ChunkId,
          record.EntryPosition);
    }

    return entry.value();
  }

  // No RemoveChunk because that will make any ID afterwards shift by -1
  // This can be mitigated by storing some kind of UUID system with exact
  // poisions instead.
  bool Remove(const Record &record) noexcept {
    if (!isRecordValid(record)) {
      return false;
    }

    Chunk *chunk = chunks[record.ChunkId];
    chunk->slots[record.EntryPosition] = std::nullopt;
    chunk->isSlotTaken.set(record.EntryPosition, false);
    --entryCount;
    return true;
  }

  std::size_t CountEntries() const noexcept { return entryCount; }
  std::size_t CountChunks() const noexcept { return chunks.size(); }

  void Clear() {
    for (Chunk *chunk : chunks) {
      delete chunk;
    }
    chunks.clear();
    entryCount = 0;
  }

private:
  constexpr static std::size_t fullSlots = options.ChunkCapacity + 1;

  using Entry = std::optional<EntryType>;

  struct Chunk {
    bool isFull = false;
    // I want to add a
    Corrade::Containers::BitArray isSlotTaken{Corrade::ValueInit,
                                              options.ChunkCapacity};
    std::array<Entry, options.ChunkCapacity> slots;

    // This is for a future insertion/removal feature
    Chunk *previous = nullptr;
    Chunk *next = nullptr;
  };

  bool isChunkIDValid(std::size_t chunkID) const noexcept {
    return chunks.size() > chunkID;
  }
  bool isEntryPositionValid(std::size_t entryPosition) const noexcept {
    return options.ChunkCapacity > entryPosition;
  }
  bool isRecordValid(const Record &record) const noexcept {
    return isChunkIDValid(record.ChunkId) &&
           isEntryPositionValid(record.EntryPosition);
  }

  Record getChunkWithEmptySlot() {
    std::size_t count = 0;
    for (Chunk *chunk : chunks) {
      ++count;
      if (chunk->isFull) {
        continue;
      }

      std::size_t slot = getNextEmptySlot(chunk);
      if (slot == fullSlots) {
        continue;
      }

      return {--count, slot};
    }

    // All the chunks are full
    Chunk *endChunk = chunks.back();
    Chunk *newChunk = new Chunk{};
    endChunk->next = newChunk;
    newChunk->previous = endChunk;
    chunks.push_back(newChunk);
    return {chunks.size() - 1, 0};
  }

  std::size_t getNextEmptySlot(Chunk *chunk) const noexcept {
    if (chunk->isFull) {
      return fullSlots;
    }

    // I feel like if I can get the bit array as char
    // or something I can do this check much faster
    // (by checking to see if all 8 bits equal to 255, SIMD style)
    for (std::size_t index = 0; index < options.ChunkCapacity; index++) {
      if (chunk->isSlotTaken[index]) {
        continue;
      }
      return index;
    }

    chunk->isFull = true;
    return fullSlots;
  }

  std::vector<Chunk *> chunks;
  std::size_t entryCount = 0;
};
}; // namespace Tourmaline::Containers
#endif
