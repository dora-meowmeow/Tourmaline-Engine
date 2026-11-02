/*
 * SPDX-FileCopyrightText: Dora "cat" <cat@thenight.club>
 * SPDX-License-Identifier: MPL-2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at http://mozilla.org/MPL/2.0/.
 */
#ifndef GUARD_TOURMALINE_CHUNKLIST_H
#define GUARD_TOURMALINE_CHUNKLIST_H

#include "../Systems/Logging.hpp"

#include "BitArray.hpp"
#include "ContainerOptions.hpp"

#include <climits>
#include <cstddef>

#include <array>
#include <optional>
#include <utility>
#include <vector>

/**
 * @file
 * @brief Pre-defined fixed sized arrays stored together. The data does not
 * move.
 */

namespace Tourmaline::Containers {
/**
 * @brief A record of where the data was placed.
 *
 * Can be used to access the data with Tourmaline::Containers::ChunkList::Get or
 * deleted with Tourmaline::Containers::ChunkList::Remove.
 */
struct ChunkListRecord {
  /**
   * @brief Which chunk stores the data.
   */
  std::size_t ChunkId = 0;
  /**
   * @brief Which slot in the chunk stores the data.
   */
  std::size_t SlotPosition = 0;

  /**
   * @brief This struct supports operator==.
   */
  friend bool operator==(const ChunkListRecord &lhs,
                         const ChunkListRecord &rhs) {
    return (lhs.ChunkId == rhs.ChunkId) &&
           (lhs.SlotPosition == rhs.SlotPosition);
  }

  /**
   * @brief This struct supports operator!=.
   */
  friend bool operator!=(const ChunkListRecord &lhs,
                         const ChunkListRecord &rhs) {
    return (lhs.ChunkId != rhs.ChunkId) ||
           (lhs.SlotPosition != rhs.SlotPosition);
  }
};

/**
 * @brief An array of chunks which themselves are pre-defined fixed sized
 * arrays.
 *
 * A decent alternative to std::vector for a medium to large amount of data.
 *
 * The data always stays in the same place even when there is a growth or an
 * entry being removed.
 *
 * @tparam EntryType What type of data should be stored.
 * @tparam See Tourmaline::Containers::ChunkListOptions.
 */
template <typename EntryType, ChunkListOptions options = {}> class ChunkList {
  constexpr static std::size_t fullSlots = options.ChunkCapacity + 1;
  using Entry = std::optional<EntryType>;
  struct Chunk {
    std::size_t leftmostEmptySlot = 0;
    BitArray isSlotTaken{{options.ChunkCapacity, false}};
    std::array<Entry, options.ChunkCapacity> slots;
  };

public:
  /**
   * @brief Forward iterator for ChunkList.
   */
  struct Iterator {
    /**
     * @brief Position specific iterator constructor.
     *
     * @param position Record to start from.
     * @param chunks Pointer to the ChunkList.
     */
    Iterator(ChunkListRecord position, std::vector<Chunk *> *chunks)
        : position(position), chunks(chunks), end(chunks->size()) {}

    /**
     * @brief End iterator Specific constructor.
     *
     * @param chunks Pointer to the ChunkList.
     *
     * The End iterator's position is deduced automatically.
     */
    Iterator(std::vector<Chunk *> *chunks)
        : chunks(chunks), end(chunks->size()) {
      position = {end, 0};
    }

    /**
     * @brief Supports copy-construction.
     */
    Iterator(const Iterator &iterator)
        : position(iterator.position), chunks(iterator.chunks),
          end(iterator.end) {}

    /**
     * @brief Supports copy-assignment.
     */
    Iterator &operator=(const Iterator &iterator) {
      position = iterator.position;
      chunks = iterator.chunks;
      end = iterator.end;
      return *this;
    };

    /**
     * @brief Supports operator* for dereferencing.
     */
    EntryType &operator*() const {
      return (*chunks)[position.ChunkId]->slots[position.SlotPosition].value();
    }

    /**
     * @brief Supports operator->.
     */
    EntryType *operator->() {
      return &(*chunks)[position.ChunkId]->slots[position.SlotPosition].value();
    }

    /**
     * @brief Supports prefix operator++.
     */
    Iterator &operator++() {
      advanceToNextValid();
      return *this;
    }

    /**
     * @brief Supports postfix operator++.
     */
    Iterator operator++(int) {
      Iterator hold = *this;
      advanceToNextValid();
      return hold;
    }

    /**
     * @brief Supports operator==.
     */
    friend bool operator==(const Iterator &lhs, const Iterator &rhs) {
      return lhs.position == rhs.position;
    }

    /**
     * @brief Supports operator!=.
     */
    friend bool operator!=(const Iterator &lhs, const Iterator &rhs) {
      return lhs.position != rhs.position;
    }

    /**
     * @brief Supports swapping.
     */
    friend void swap(Iterator &first, Iterator &second) {
      using std::swap;
      swap(first.position, second.position);
      swap(first.chunks, second.chunks);
      swap(first.end, second.end);
    }

    /**
     * @brief Where in the ChunkList the iterator is currently in.
     */
    ChunkListRecord position;

  private:
    std::vector<Chunk *> *chunks;
    std::size_t end;
    void advanceToNextValid() {
      do {
        if (++position.SlotPosition < options.ChunkCapacity) {
          continue;
        }
        ++position.ChunkId;
        position.SlotPosition = 0;

        // This means that the iterator hit the end of this range
        if (position.ChunkId >= end) {
          position.ChunkId = end;
          return;
        }
      } while (!(*chunks)[position.ChunkId]
                    ->slots[position.SlotPosition]
                    .has_value());
    }
  };

  /**
   * @brief Default constructor.
   *
   * @warning This will output a log with a level of Critical(which will cause
   * std::terminate to be called), if ChunkListOptions::ChunkCapacity
   * is set to 0.
   */
  ChunkList() {
    Systems::Logging::Log("ChunkListOptions::ChunkCapacity cannot be 0 or "
                          "less! Terminating...",
                          "ChunkList", Systems::Logging::Critical,
                          options.ChunkCapacity == 0);
    chunks.push_back(new Chunk{});
  }

  /**
   * @brief Default destructor.
   */
  ~ChunkList() { Clear(); }

  /**
   * @brief Copying is not allowed.
   *
   * No copying since the data is owned by the instance of this class. Deleting
   * to mitigate shallow copying.
   */
  ChunkList(const ChunkList &) = delete;

  /**
   * @brief Copying is not allowed.
   *
   * No copying since the data is owned by the instance of this class. Deleting
   * to mitigate shallow copying.
   */
  ChunkList &operator=(const ChunkList &) = delete;

  /**
   * @brief Moving is allowed.
   */
  ChunkList(ChunkList &&linkedChunk) noexcept
      : chunks(std::move(linkedChunk.chunks)),
        entryCount(linkedChunk.entryCount) {
    linkedChunk.entryCount = 0;
  }

  /**
   * @brief Moving is allowed.
   */
  ChunkList &operator=(ChunkList &&rightHandSide) noexcept {
    if (this == &rightHandSide) {
      return *this;
    }
    Clear();
    this->entryCount = rightHandSide.entryCount;
    chunks = std::move(rightHandSide.chunks);
    rightHandSide.entryCount = 0;

    return *this;
  }

  /**
   * @brief Inserts data into a chunk by copying.
   *
   * @param entry Data to copy.
   *
   * @return Location of that data as a record.
   */
  ChunkListRecord Insert(const EntryType &entry) noexcept {
    ChunkListRecord chunkPair = getChunkWithEmptySlot();
    Chunk *chunk = chunks[chunkPair.ChunkId];
    std::size_t slot = chunkPair.SlotPosition;

    chunk->slots[slot] = entry;
    chunk->isSlotTaken.SetBit(slot, true);
    chunk->leftmostEmptySlot = chunk->isSlotTaken.FindFirst(0);
    ++entryCount;

    return chunkPair;
  }

  /**
   * @brief Fetches data at a given location and returns a reference to it.
   *
   * @param record A record containg the location information.
   *
   * @return Reference to the data.
   */
  EntryType &Get(const ChunkListRecord &record) {
    Systems::Logging::Log(
        "ChunkList was given a record that was out of bounds! Throwing...",
        "ChunkList/Get", Systems::Logging::Error, !isRecordValid(record));

    // Not using at since we already did bounds check above
    Entry &entry = chunks[record.ChunkId]->slots[record.SlotPosition];
    if (!entry.has_value()) {
      Systems::Logging::LogFormatted(
          "There is no valid data at chunk {} slot {}! Throwing...",
          "ChunkList/Get", Systems::Logging::Error, record.ChunkId,
          record.SlotPosition);
    }

    return entry.value();
  }

  /**
   * @brief Checks to see if the record points to a slot with a value.
   *
   * @param record A record containg the location information.
   *
   * @return True, if the record points to a slot with a value. False, if either
   * slot doesn't contain data OR the record is invalid.
   */
  bool Has(const ChunkListRecord &record) const noexcept {
    // Not using at since we already did bounds check above
    if (!isRecordValid(record)) {
      return false;
    }

    return chunks[record.ChunkId]->slots[record.SlotPosition].has_value();
  }

  // No RemoveChunk because that will make any ID afterwards shift by -1
  // This can be mitigated by storing some kind of UUID system with exact
  // poisions instead.
  /**
   * @brief Deletes data at a given location.
   *
   * @param record A record containg the location information.
   *
   * @return True if the data was destroyed, false otherwise.
   */
  bool Remove(const ChunkListRecord &record) noexcept {
    if (!isRecordValid(record)) {
      return false;
    }

    Chunk *chunk = chunks[record.ChunkId];
    chunk->slots[record.SlotPosition] = std::nullopt;
    chunk->isSlotTaken.SetBit(record.SlotPosition, false);
    --entryCount;

    if (chunk->leftmostEmptySlot > record.SlotPosition) {
      chunk->leftmostEmptySlot = record.SlotPosition;
    }

    if (leftmostEmptyChunk > record.ChunkId) {
      leftmostEmptyChunk = record.ChunkId;
    }

    return true;
  }

  /**
   * @brief How many total entries are in this container.
   *
   * @return Number of entries in this container (Filled slots in each chunk).
   */
  std::size_t CountEntries() const noexcept { return entryCount; }

  /**
   * @brief How many chunks are in this container.
   *
   * @return Number of chunks currently allocated.
   */
  std::size_t CountChunks() const noexcept { return chunks.size(); }

  /**
   * @brief Clears the container and every entry inside it.
   */
  void Clear() {
    for (Chunk *chunk : chunks) {
      delete chunk;
    }
    chunks.clear();
    entryCount = 0;
  }

  /**
   * @brief Gives an iterator that points to the first slot with data in it.
   *
   * @return An iterator.
   */
  Iterator begin() { return Iterator(getFirstFullSlot(), &chunks); }

  /**
   * @brief Gives an iterator that points to the chunk one after the last valid chunk.
   *
   * @return An iterator with data outside of the container.
   */
  Iterator end() { return Iterator(&chunks); }

private:
  bool isChunkIDValid(std::size_t chunkID) const noexcept {
    return chunks.size() > chunkID;
  }
  bool isSlotPositionValid(std::size_t entryPosition) const noexcept {
    return options.ChunkCapacity > entryPosition;
  }
  bool isRecordValid(const ChunkListRecord &record) const noexcept {
    return isChunkIDValid(record.ChunkId) &&
           isSlotPositionValid(record.SlotPosition);
  }

  ChunkListRecord getFirstFullSlot() const noexcept {
    std::size_t size = chunks.size(), full = 0;
    for (std::size_t count = 0; count < size; count++) {
      full = chunks[count]->isSlotTaken.FindFirst(1);
      if (full >= options.ChunkCapacity) {
        continue;
      }

      return {count, full};
    }
    return {size, 0};
  }

  ChunkListRecord getChunkWithEmptySlot() noexcept {
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
