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

#include "../Containers/Hashmap.hpp"
#include "../Systems/ECS.hpp"
#include "Input.hpp"

#include "Corrade/Containers/String.h"
#include "Magnum/Magnum.h"
#include "Magnum/Platform/GlfwApplication.h"
#include "Magnum/Timeline.h"

#include <cstdint>
#include <exception>

namespace Tourmaline::Game {
using namespace Magnum::Math::Literals;

/**
 * @file
 * @brief The fundamentals to make a game.
 */

/**
 * @brief The class that contains all of the game logic.
 * Inherit this class publicly, it will come with most things pre-set.
 * You will just need to override any virtual functions you need.
 */
class Program : private Magnum::Platform::GlfwApplication {
public:
  /// @brief Program configuration.
  struct Config {
    /**
     * @brief Types of modes a window could be.
     */
    enum class WindowMode : uint32_t {
      Windowed = 1 << 10,
      Fullscreen = 1 << 0,
      Borderless = 1 << 1,
      Maximized = 1 << 4,
    };

    /// @brief Title of the window to be created.
    Corrade::Containers::String windowTitle{"Game Window"};

    /// @brief Dimensions of the window.
    Magnum::Vector2i windowSize{800, 600};

    /**
     *-@brief Maximum/Desired frame rate per second. When set to 0 WITH VSYNC
     * DISABLED (See Tourmaline::Game::Program::Config::vsyncEnabled),
     * the framerate will not be capped.
     */
    uint64_t desiredFrameRate = 0;

    /// @brief See Tourmaline::Game::Program::Config::WindowMode.
    WindowMode windowMode = WindowMode::Windowed;

    /// @brief Set to false to disable Vsync, set to true to enable Vsync.
    bool vsyncEnabled = 1;
  };

  /// @cond
  explicit Program()
      : Magnum::Platform::Application{Arguments{arguments.argc, arguments.argv},
                                      Configuration{}} {};
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
   * @brief Applies the changes done on Tourmaline::Game::Program::config
   * to the window itself.
   *
   * @warning This feature only works with GLFW which is selected by default.
   * It will not work if you modify Tourmaline to use SDL2 or anything else.
   */
  void ApplyNewConfig();

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
   * This function is called **BEFORE** ECS::World takes its step.
   */
  virtual void OnStep();

  /**
   * @brief A function called right before a crash due to an unhandled
   * exception.
   *
   * If, during your game loop, an exception is thrown and uncaught,
   * Game::Program will catch it as a last line of defense. Once the exception is
   * caught, this function will run.
   *
   * @param exception The exception itself (for diagnostics purposes).
   *
   * @warning Unless you are handling exceptions, it is HIGHLY suggested that
   * you define this function. A crash due to an uncaught exception could result
   * in the player's progress being unsaved before the crash! We also strongly
   * recommend setting Tourmaline::Systems::Logging::TerminationFunction for the
   * same reason.
   */
  virtual void OnCrash(const std::exception &exception);

  /**
   * @brief Destruction/Last step of the program.
   *
   * @return If this function returns true, the program will safely exit,
   * otherwise execution will continue.
   */
  virtual bool OnExit();

  /**
   * @brief Fetches key inputs and states of keys.
   *
   * @param keyType The key to track. See @ref Tourmaline::Game::Input::KeyType.
   *
   * @return Returns a view to the state of a key.
   */
  [[nodiscard("Unnecessary call to GetKey")]]
  const Input::Key &GetKey(const Input::KeyType &keyType);

  /**
   * @brief Allows access to the command line arguments.
   * If you desire to use command line arguments, you must set these.
   *
   * @note If unset, argc will be set to 1, and argv will contain the string
   * "empty".
   * @warning It is highly advised to set these BEFORE the construction
   * of the game program.
   */
  struct Args {
    /// @brief Argument count.
    int argc;

    /// @brief Argument storage.
    char **argv;
  };

  /// @brief See Tourmaline::Game::Program::Args.
  static Args arguments;

  /// @brief Configurations for the program. See
  /// Tourmaline::Game::Program::Config.
  Config config;

  /// @brief Built-in ECS system. See Tourmaline::Systems::ECS::World
  /// for more info on how to use it.
  Systems::ECS::World ECS;

  /// @brief Time it took to draw the last frame in seconds.
  const float &deltaTime = _deltaTime;

  /// @brief Aspect ratio of the window.
  const float &aspectRatio = _aspectRatio;

private:
  void initialize();
  void advanceKeyEvents();

  void drawEvent() override;
  void viewportEvent(ViewportEvent &event) override;
  void exitEvent(ExitEvent &event) override;
  void keyPressEvent(KeyEvent &event) override;
  void keyReleaseEvent(KeyEvent &event) override;

  // Magnum
  Magnum::Timeline timeline;
  Configuration magnumConfig;

  // Internal data
  float _deltaTime = 0;
  float _aspectRatio = 0;
  Containers::Hashmap<Input::KeyType, Input::Key> _inputTrack;
  std::vector<Input::KeyType> _advanceKeys;

  // Empty data incase the dev doesn't want to pass arguments
  inline static char *_argv = (char *)"empty";
  inline static int _argc = 1;
};
} // namespace Tourmaline::Game
#endif
