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

#include "BitArray.hpp"
#include "ContainerOptions.hpp"

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
    chunk->isSlotTaken.SetBit(slot, true);
    chunk->leftmostEmptySlot = chunk->isSlotTaken.FindFirst(0);
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
    chunk->isSlotTaken.SetBit(record.EntryPosition, false);
    --entryCount;

    if (chunk->leftmostEmptySlot > record.EntryPosition) {
      chunk->leftmostEmptySlot = record.EntryPosition;
    }

    if (leftmostEmptyChunk > record.ChunkId) {
      leftmostEmptyChunk = record.ChunkId;
    }

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
    std::size_t leftmostEmptySlot = 0;
    BitArray isSlotTaken{{options.ChunkCapacity, false}};
    std::array<Entry, options.ChunkCapacity> slots;
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

  Record getChunkWithEmptySlot() noexcept {
    std::size_t size = chunks.size(), empty = 0;
    for (std::size_t count = leftmostEmptyChunk; count < size; count++) {
      empty = chunks[count]->leftmostEmptySlot;
      if (empty >= options.ChunkCapacity) {
        continue;
      }

      return {count, empty};
    }

    // All the chunks are full
    chunks.push_back(new Chunk{});
    leftmostEmptyChunk = size;
    return {size, 0};
  }

  std::vector<Chunk *> chunks;
  std::size_t entryCount = 0, leftmostEmptyChunk = 0;
};
}; // namespace Tourmaline::Containers
#endif
