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

using namespace Tourmaline::Systems;

// This is what happens when it takes you 50 years to implement
// reflections to a language
std::array<const std::string, 6> Logging::LogLevelToString{
    "Critical", "Error", "Warning", "Info", "Debug", "Trace"};
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
    std::string output =
        std::format("[{}@{}] {}\n",
                    Logging::LogLevelToString[static_cast<size_t>(severity)],
                    position, message);

    std::print("{}", output);
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
