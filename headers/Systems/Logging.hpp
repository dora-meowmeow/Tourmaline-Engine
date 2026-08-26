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

#include "Corrade/Containers/Function.h"
#include "Corrade/Containers/String.h"
#include "Corrade/Containers/StringView.h"
#include "Corrade/Utility/Format.h"
#include "Corrade/Utility/FormatStl.h"

#include <fstream>

/**
 * @file
 * @brief Built-in logging capacities.
 */

namespace Tourmaline::Systems {
/**
 * @brief Static class used for logging
 */
class Logging {
public:
  /**
   * @brief Log severity
   *
   * @note when LogLevel::Error is given as severity to a logging function. The
   * function will throw an std::runtime_error.
   *
   * @warning when LogLevel::Critical is given as severity to a logging
   * function. The function will trigger std::terminate and shutdown the
   * software.
   */
  enum LogLevel { Critical, Error, Warning, Info, Debug, Trace };

  /**
   * @brief When executed, logged messages will also be written to
   * a file.
   * @param File Optional path for the log file. If not specified, the file name
   * will be `./Tourmaline-yy-mm-dd.txt`
   */
  static void LogToFile(Corrade::Containers::String File = "");

  /**
   * @brief Non-formatting logging function.
   *
   * @param message The body of the message.
   * @param position Where this log originates from. Specifying this, while
   * optional, is highly recommended.
   * @param severity How severe this log message is. See
   * Tourmaline::Systems::Logging::LogLevel.
   * @param assertion The log message will only be sent when this is set to
   * true, otherwise it will be ignored.
   */
  static void Log(Corrade::Containers::StringView message,
                  Corrade::Containers::StringView position = "Unknown",
                  LogLevel severity = LogLevel::Info, bool assertion = true);

  /**
   * @brief Formatting logging function.
   *
   * @tparam Args args arguments to format the log with.
   *
   * @param format The body of the message, `{}` can be used
   * to substitute for arguments.
   * @param position Where this log originates from.
   * @param severity How severe this log message is. See @ref
   * Tourmaline::Systems::Logging::LogLevel.
   * @param args Arguments to use with format.
   */
  template <class... Args>
  static void LogFormatted(const char *format, const char *position,
                           LogLevel severity, const Args &...args) {
    static Corrade::Containers::String output{Corrade::ValueInit, 4096};
    std::size_t size = Corrade::Utility::formatInto(output, format, args...);
    Log(Corrade::Containers::StringView{output.begin(), size}, position,
        severity);
  }

  /**
   * @brief Function to run before termination due to
   * Logging::LogLevel::Critical.
   *
   * The function will be called right before std::terminate is called due to
   * Logging::LogLevel::Critical level log.
   *
   * @note It is heavily suggested that you set this function as a way to save
   * program state before critical error termination.
   *
   * @note This can be useful for example taking a save of the game right
   * before a crash.
   */
  static Corrade::Containers::Function<void()> TerminationFunction;

private:
  static std::fstream File;
  static const char *LogLevelToColour[Trace + 1];
  static const char *LogLevelToString[Trace + 1];
};
} // namespace Tourmaline::Systems
#endif
