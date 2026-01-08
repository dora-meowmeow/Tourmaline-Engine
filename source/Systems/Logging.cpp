/*
 * SPDX-FileCopyrightText: Dora "cat" <cat@thenight.club>
 * SPDX-License-Identifier: MPL-2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "../../headers/Systems/Logging.hpp"

#include <cerrno>
#include <chrono>
#include <cstddef>
#include <cstring>
#include <format>
#include <fstream>
#include <print>
#include <stdexcept>
#include <string>
#include <utility>

using namespace Tourmaline::Systems;

// This is what happens when it takes you 50 years to implement
// reflections to a language
std::array<std::pair<const std::string, const std::string>, 6>
    Logging::LogLevelToString{std::pair{"Critical", "[0;31m"},
                              {"Error", "[0;91m"},
                              {"Warning", "[0;33m"},
                              {"Info", "[0;37m"},
                              {"Debug", "[0;92m"},
                              {"Trace", "[0;36m"}};
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

void Logging::Log(const std::string &message, const std::string &position,
                  Logging::LogLevel severity, bool assertion) {
  if (assertion) [[likely]] {
    auto loglevelData =
        Logging::LogLevelToString[static_cast<size_t>(severity)];
    std::string output =
        std::format("[{}@{}] {}\n", loglevelData.first, position, message);

    std::print("\033{} {}\033[0m", loglevelData.second, output);
    if (Logging::File.is_open()) {
      Logging::File.write(output.c_str(), output.size());
      Logging::File.flush(); // Terrible but necessary sadly
    }

    // Error and Critical
    if (severity < Logging::LogLevel::Warning) {
      throw std::runtime_error(output);
    }
  }
}
