/*
 * SPDX-FileCopyrightText: Dora "cat" <cat@thenight.club>
 * SPDX-License-Identifier: MPL-2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at http://mozilla.org/MPL/2.0/.
 */
#ifndef GUARD_TOURMALINE_LOGGING_H
#define GUARD_TOURMALINE_LOGGING_H

#include "Corrade/Containers/String.h"
#include "Corrade/Containers/StringView.h"
#include "Corrade/Utility/Format.h"
#include "Corrade/Utility/FormatStl.h"

#include <fstream>

namespace Tourmaline::Systems {
/**
 * @file
 * @brief Static class used for logging
 */
class Logging {
public:
  /**
   * @brief Log severity
   *
   * @note when Log::Level::Error is given as severity. The logging function
   * will throw an std::runtime_error.
   *
   * @note when Log::Level::Critical is given as severity. The logging function
   * will trigger std::terminate and shutdown the software.
   */
  enum LogLevel { Critical, Error, Warning, Info, Debug, Trace };

  /**
   * @brief When executed, the logged messages will also be written to
   * a file.
   * @param File optional path for the log file. If not specified the file name
   * will be `./Tourmaline-yy-mm-dd.txt`
   */
  static void LogToFile(Corrade::Containers::String File = "");

  /**
   * @brief Non-formating logging function.
   *
   * @param message The body of the message.
   * @param position Where this log originates from. Specifying this while
   * optional is heavily suggested.
   * @param severity How severe is this log message, see @ref
   * Tourmaline::Systems::Logging::LogLevel .
   * @param assertion when set to `true` the log message will actually be sent,
   * otherwise it will be ignored.
   */
  static void Log(Corrade::Containers::StringView message,
                  Corrade::Containers::StringView position = "Unknown",
                  LogLevel severity = LogLevel::Info, bool assertion = true);

  /**
   * @brief Formating logging function.
   *
   * @param format The body of the message, when `{}` is found it will be used
   * to substitute with an argument.
   * @param position Where this log originates from.
   * @param severity How severe is this log message, see @ref
   * Tourmaline::Systems::Logging::LogLevel .
   * @tparam Args args arguments to format the log with.
   */
  template <class... Args>
  static void LogFormatted(const char *format, const char *position,
                           LogLevel severity, const Args &...args) {
    static Corrade::Containers::String output{Corrade::ValueInit, 4096};
    std::size_t size = Corrade::Utility::formatInto(output, format, args...);
    Log(Corrade::Containers::StringView{output.begin(), size}, position,
        severity);
  }

private:
  static std::fstream File;
  static const char *LogLevelToColour[Trace + 1];
  static const char *LogLevelToString[Trace + 1];
};
} // namespace Tourmaline::Systems
#endif
