/*
 * SPDX-FileCopyrightText: Dora "cat" <cat@thenight.club>
 * SPDX-License-Identifier: MPL-2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <array>

// Fuck this language unironically
#include <cstdio>
#include <unistd.h>

#include "Corrade/Containers/String.h"
#include "Corrade/TestSuite/Compare/File.h"
#include "Corrade/TestSuite/Tester.h"

#include "Systems/Logging.hpp"
#include "Tests.hpp"

using namespace Tourmaline;
using namespace Systems;
using namespace Corrade;

Corrade::Containers::String Tests::fakeStdOutName = "fakestdout.txt",
                            position = "LoggingTest";

void Tests::Logging_ConsoleAndFileSync() {
  Corrade::Containers::String outfile = "Test_WritingToFile.txt";
  try {
    Logging::LogToFile(outfile, true);
    Helper_SwitchSTDOUT(true);

    struct message {
      Corrade::Containers::String content;
      Logging::LogLevel level;
    };

    std::array<message, 4> messages{{
        {"Hello!", Logging::Trace},
        {"bleepbloop :)", Logging::Debug},
        {"Wooo (^.O.^)", Logging::Info},
        {"AAAAAAAAAAAA!!!!! PANIC!!!!! PANIC!!!! AAAAAAAAAAAAAAA",
         Logging::Warning},
    }};

    for (const message &msg : messages) {
      Logging::Log(msg.content, position, msg.level);
    }

    Helper_SwitchSTDOUT(false);
    CORRADE_COMPARE_AS(outfile, Tests::fakeStdOutName,
                       Corrade::TestSuite::Compare::File);
  } catch (...) {
    CORRADE_EXPECT_FAIL(
        "The fail could be due to a permission issue. Skipping.");
    CORRADE_VERIFY(false);
  }
}

void Tests::Logging_FileSwitching() {
  Corrade::Containers::String outfile = "Test_FileSwitching.txt",
                              otherfile =
                                  "Test_FileSwitchingTotallyUnrealted.txt";
  // Just so the console stays clean
  Helper_SwitchSTDOUT(true);
  try {
    // Can we write?
    Logging::LogToFile(outfile);
    Logging::Log("test", position);

    // Can we switch?
    Logging::LogToFile(otherfile, true);
    Logging::Log("Narval blast!", position);

    // Can we switch back and clear?
    Logging::LogToFile(outfile, true);
    Logging::Log("Narval blast!", position);
  } catch (...) {
    // If anything throws, it fails automatically. No need to HCF.
    Helper_SwitchSTDOUT(false);
    CORRADE_EXPECT_FAIL(
        "The fail could be due to a permission issue. Skipping.");
    CORRADE_VERIFY(false);
    return;
  }

  Helper_SwitchSTDOUT(false);
  CORRADE_COMPARE_AS(outfile, otherfile, Corrade::TestSuite::Compare::File);
}

void Tests::Logging_ErrorAndCritical() {
  // Just so the console stays clean
  Helper_SwitchSTDOUT(true);
  bool thrown = false, triedToTerminate = false,
       calledTerminateFunction = false;

  // Error
  try {
    Logging::Log("Error! Throwing time!", position, Logging::Error);
    CORRADE_FAIL("Logging::Error should've thrown!");
    return;
  } catch (...) {
    thrown = true;
  }

  // Critical
  std::terminate_handler oldTerminate = std::get_terminate();
  std::set_terminate([]() { throw std::runtime_error(""); });
  Logging::TerminationFunction = [&calledTerminateFunction]() {
    calledTerminateFunction = true;
  };

  try {
    Logging::Log("Critical error! it's not a phase mom", position,
                 Logging::Critical);
    CORRADE_FAIL("Logging::Critical should've terminated!");
    return;
  } catch (...) {
    triedToTerminate = true;
  }

  // Making sure termination function also works
  if (!calledTerminateFunction) {
    CORRADE_FAIL("TerminateFunction was not called!");
  }
  std::set_terminate(oldTerminate);

  Helper_SwitchSTDOUT(false);
  CORRADE_VERIFY(thrown && triedToTerminate && calledTerminateFunction);
}

void Tests::Helper_SwitchSTDOUT(bool fakeSTD) {
  static int stdOut = dup(STDOUT_FILENO);

  fflush(stdout);

  if (fakeSTD) {
    FILE *fakeStdOut = fopen(Tests::fakeStdOutName.data(), "w");
    dup2(fileno(fakeStdOut), STDOUT_FILENO);
    fclose(fakeStdOut);
    return;
  }
  dup2(stdOut, STDOUT_FILENO);
}
