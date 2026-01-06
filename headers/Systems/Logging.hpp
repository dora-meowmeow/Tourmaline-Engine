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
#include <array>
#include <fstream>
#include <string>

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
  static void Log(const std::string &message,
                  const std::string &position = "Unknown",
                  LogLevel severity = LogLevel::Info, bool assertion = true);

private:
  static std::fstream File;
  static std::array<const std::string, 6> LogLevelToString;
};
} // namespace Tourmaline::Systems
#endif
