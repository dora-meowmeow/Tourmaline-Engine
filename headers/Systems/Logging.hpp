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

#include <fstream>

namespace Tourmaline::Systems {
class Logging {
public:
  enum LogLevel { Critical, Error, Warning, Info, Debug, Trace };

  static void LogToFile(Corrade::Containers::String File = "");
  static void Log(Corrade::Containers::StringView message,
                  Corrade::Containers::StringView position = "Unknown",
                  LogLevel severity = LogLevel::Info, bool assertion = true);

  template <class... Args>
  static void LogFormatted(const char *format,
                           Corrade::Containers::StringView position,
                           LogLevel severity, const Args &...args) {
    Corrade::Containers::String formatted =
        Corrade::Utility::format(format, args...);
    Log(formatted, position, severity);
  }

private:
  static std::fstream File;
  static const char *LogLevelToColour[LogLevel::Trace + 1];
  static const char *LogLevelToString[LogLevel::Trace + 1];
};
} // namespace Tourmaline::Systems
#endif
