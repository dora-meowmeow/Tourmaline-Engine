/*
 * SPDX-FileCopyrightText: Dora "cat" <cat@thenight.club>
 * SPDX-License-Identifier: MPL-2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at http://mozilla.org/MPL/2.0/.
 */
#ifndef GUARD_TOURMALINE_GAME_H
#define GUARD_TOURMALINE_GAME_H

#include "../Systems/ECS.hpp"

#include "Corrade/Containers/String.h"
#include "Magnum/Magnum.h"
#include "Magnum/Platform/GlfwApplication.h"
#include "Magnum/Tags.h"

namespace Tourmaline::Game {
using namespace Magnum::Math::Literals;

/**
 * @file
 * @brief The fundamentals to make a game.
 */

/**
 * @brief The class that contains all of the game logic.
 * Inherit this class publically, it will come with most things pre-set.
 * You will just need to override any virtual functions you need.
 */
class Program : private Magnum::Platform::GlfwApplication {
public:
  /// @brief Program configuration.
  struct Config {
    /// @brief Title of the window created.
    Corrade::Containers::String windowTitle{"Game Window"};
    /// @brief Dimensions of the window.
    Magnum::Vector2i windowSize{800, 600};
    /// @brief Maximum/Desired frame rate per second.
    uint64_t desiredFrameRate = 60;
  };

  /// @cond
  explicit Program()
      : Magnum::Platform::Application{Arguments{arguments.argc, arguments.argv},
                                      Magnum::NoCreate} {};
  /// @endcond

  /**
   * @brief Starts the program.
   *
   * @param conf Configurations for the program.
   *
   * @return Exit code. Anything except 0 means an error.
   */
  int Run(const Config &conf);

  /**
   * @brief Initialization/Setup step of the program.
   *
   * It is heavily advised to override this function, as you will need to
   * initialize/setup many things before execution.
   */
  virtual void OnStart();

  /**
   * @brief A function called per frame.
   *
   * This function is called **BEFORE** ECS::World takes it's step.
   */
  virtual void OnStep();

  /**
   * @brief Destruction/Last step of the program.
   *
   * @return if returns true the program will safely exit, otherwise execution
   * will continue.
   */
  virtual bool OnExit();

  /// @brief Built-in ECS system. See Tourmaline::Systems::ECS::World
  /// for more info on how to use it.
  Systems::ECS::World ECS;

  /**
   * @brief Allows access to the command line arguments.
   * If you desire to use command line arguments, you must set these.
   *
   * @note If unset argc will be set to 1, and argv will have "empty".
   * @warning It is heavily advised these are set BEFORE construction
   * the game program.
   */
  struct Args {
    /// @brief Argument count.
    int argc;

    /// @brief Argument storage.
    char **argv;
  } static arguments;

private:
  void initialize();
  void drawEvent() override;
  void exitEvent(ExitEvent &event) override;

  Config config;
  // Empty data incase the dev doesn't want to pass arguments
  inline static char *_argv = (char *)"empty";
  inline static int _argc = 1;
};
} // namespace Tourmaline::Game
#endif
