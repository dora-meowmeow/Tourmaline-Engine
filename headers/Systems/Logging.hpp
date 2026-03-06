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

#include "Corrade/Containers/Array.h"

#include <fstream>
#include <string_view>

namespace Tourmaline::Systems {
class Logging {
public:
  enum class LogLevel {
    Critical = 0,
    Error = 1,
    Warning = 2,
    Info = 3,
    Debug = 4,
    Trace = 5
  };

  static void LogToFile(std::string File = "");
  static void Log(std::string_view message,
                  std::string_view position = "Unknown",
                  LogLevel severity = LogLevel::Info, bool assertion = true);

private:
  static std::fstream File;
  static Corrade::Containers::Array<
      std::pair<const std::string, const std::string>>
      LogLevelToString;
};
} // namespace Tourmaline::Systems
#endif
