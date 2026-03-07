/*
 * SPDX-FileCopyrightText: Dora "cat" <cat@thenight.club>
 * SPDX-License-Identifier: MPL-2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "Systems/Logging.hpp"

#include "Corrade/Containers/String.h"
#include "Corrade/Containers/StringView.h"
#include "Corrade/Tags.h"
#include "Corrade/Utility/Format.h"

#include <cerrno>
#include <chrono>
#include <cstddef>
#include <cstring>
#include <exception>
#include <format>
#include <fstream>
#include <print>
#include <stdexcept>
#include <string_view>

using namespace Tourmaline::Systems;
using namespace Corrade::Containers;
using namespace Corrade::Utility;

// This is what happens when it takes you 50 years to implement
// reflections to a language
const char *Logging::LogLevelToColour[Logging::LogLevel::Trace + 1]{
    "[0;31m", "[0;91m", "[0;33m", "[0;37m", "[0;92m", "[0;36m"};
const char *Logging::LogLevelToString[Logging::LogLevel::Trace + 1]{
    "Critical", "Error", "Warning", "Info", "Debug", "Trace"};
std::fstream Logging::File;

void Logging::LogToFile(String File) {
  if (File == "") {
    const auto now = std::chrono::system_clock::now();
    std::chrono::year_month_day ymd{std::chrono::floor<std::chrono::days>(now)};
    File = String{Corrade::ValueInit, 128};
    formatInto(File, "Tourmaline-{}-{}-{}.txt", static_cast<int>(ymd.year()),
               static_cast<unsigned>(ymd.month()),
               static_cast<unsigned>(ymd.day()));
  }
  Logging::File.open(File.data(), std::fstream::out);

  if (Logging::File.fail()) {
    String error =
        format("FAILED! Could not open or create the file: {}! Error: {}", File,
               strerror(errno));
    throw std::runtime_error(error.data());
  }
}

void Logging::Log(StringView message, StringView position,
                  Logging::LogLevel severity, bool assertion) {
  if (assertion) {
    static String output{Corrade::ValueInit,
                         4096}; // This is done to stop allocations
    std::size_t formattedSize = formatInto(
        output, "[{}@{}] {}\n", LogLevelToString[severity], position, message);

    std::print(
        "\033{} {}\033[0m", LogLevelToColour[severity],
        std::string_view{output.begin(), output.begin() + formattedSize});
    if (Logging::File.is_open()) {
      Logging::File.write(output.data(), formattedSize);
      Logging::File.flush(); // Terrible but necessary sadly
    }

    if (severity == Logging::LogLevel::Error) {
      throw std::runtime_error(output.data());
    }

    if (severity == Logging::LogLevel::Critical) {
      std::terminate();
    }
  }
}
