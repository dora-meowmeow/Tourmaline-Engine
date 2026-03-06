/*
 * SPDX-FileCopyrightText: Dora "cat" <cat@thenight.club>
 * SPDX-License-Identifier: MPL-2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "Systems/Logging.hpp"
#include "Corrade/Containers/Array.h"

#include <Corrade/Tags.h>
#include <cerrno>
#include <chrono>
#include <cstddef>
#include <cstring>
#include <exception>
#include <format>
#include <fstream>
#include <iterator>
#include <print>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

using namespace Tourmaline::Systems;
using namespace Corrade::Containers;

// This is what happens when it takes you 50 years to implement
// reflections to a language
Array<std::pair<const std::string, const std::string>>
    Logging::LogLevelToString{Corrade::InPlaceInit,
                              {std::pair{"Critical", "[0;31m"},
                               {"Error", "[0;91m"},
                               {"Warning", "[0;33m"},
                               {"Info", "[0;37m"},
                               {"Debug", "[0;92m"},
                               {"Trace", "[0;36m"}}};
std::fstream Logging::File;

void Logging::LogToFile(std::string File) {
  if (File == "") {
    const auto now = std::chrono::system_clock::now();
    File = std::format("Tourmaline-{:%Y-%j}.txt", now);
  }
  Logging::File.open(File, std::fstream::out);

  if (Logging::File.fail()) {
    throw std::runtime_error("FAILED! Could not open or create the file: " +
                             File + "!\n" + strerror(errno));
  }
}

void Logging::Log(std::string_view message, std::string_view position,
                  Logging::LogLevel severity, bool assertion) {
  if (assertion) [[likely]] {
    static std::string
        output; // This is done to stop allocations per std::format
    const auto &loglevelData =
        Logging::LogLevelToString[static_cast<size_t>(severity)];
    std::format_to(std::back_inserter(output), "[{}@{}] {}\n",
                   loglevelData.first, position, message);

    std::print("\033{} {}\033[0m", loglevelData.second, output);
    if (Logging::File.is_open()) {
      Logging::File.write(output.c_str(), output.size());
      Logging::File.flush(); // Terrible but necessary sadly
    }

    if (severity == Logging::LogLevel::Error) {
      throw std::runtime_error(output);
    }

    if (severity == Logging::LogLevel::Critical) {
      std::terminate();
    }

    output.clear();
  }
}
