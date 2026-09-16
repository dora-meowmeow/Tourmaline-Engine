/*
 * SPDX-FileCopyrightText: Dora "cat" <cat@thenight.club>
 * SPDX-License-Identifier: MPL-2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at http://mozilla.org/MPL/2.0/.
 */
#ifndef GUARD_TOURMALINE_INPUT_H
#define GUARD_TOURMALINE_INPUT_H

#include "Magnum/Platform/GlfwApplication.h"
/**
 * @file
 * @brief Keyboard input related types
 */
namespace Tourmaline::Game::Input {
/**
 * @brief States a key can be in.
 *
 * @note Released event can fire without a pressed event firing. This usually
 * happens when the program is focused while holding a key down. This rarely
 * causes and issue but still should be noted.
 */
enum class KeyState {
  Inactive = 0,
  Pressed = 1,
  Released = 2,
  Held = 3,
};

/**
 * @brief Keycodes that GLFW uses.
 *
 * @see
 * [Magnum::Platform::GlfwApplication::Key](https://doc.magnum.graphics/magnum/classMagnum_1_1Platform_1_1GlfwApplication.html#a57b9583804a678cba5aea08b227061cc)
 */
using KeyType = Magnum::Platform::GlfwApplication::Key;

/**
 * @brief A class that stores the states of each key.
 */
class Key {
public:
  /// See Tourmaline::Game::Input::KeyState
  KeyState state;

  /// See Tourmaline::Game::Input::KeyType
  KeyType type;

  /**
   * @brief Quality of Life function to check state.
   *
   * @return Returns true if state is equal to
   * [KeyState::Inactive](@ref Tourmaline::Game::Input::KeyState), otherwise
   * returns false.
   */
  bool IsInactive() const noexcept;

  /**
   * @brief Quality of Life function to check state.
   *
   * @return Returns true if state __**IS NOT**__ equal to
   * [KeyState::Inactive](@ref Tourmaline::Game::Input::KeyState), otherwise
   * returns false.
   */
  bool IsActive() const noexcept;

  /**
   * @brief Quality of Life function to check state.
   *
   * @return Returns true if state is equal to either
   * [KeyState::Pressed](@ref Tourmaline::Game::Input::KeyState) or
   * [KeyState::Held](@ref Tourmaline::Game::Input::KeyState), otherwise
   * returns false.
   */
  bool IsDown() const noexcept;

  /**
   * @brief Quality of Life function to check state.
   *
   * @return Returns true if state is equal to
   * [KeyState::Pressed](@ref Tourmaline::Game::Input::KeyState), otherwise
   * returns false.
   */
  bool IsPressed() const noexcept;

  /**
   * @brief Quality of Life function to check state.
   *
   * @return Returns true if state is equal to
   * [KeyState::Released](@ref Tourmaline::Game::Input::KeyState), otherwise
   * returns false.
   */
  bool IsReleased() const noexcept;

  /**
   * @brief Quality of Life function to check state.
   *
   * @return Returns true if state is equal to
   * [KeyState::Held](@ref Tourmaline::Game::Input::KeyState), otherwise returns
   * false.
   */
  bool IsHeld() const noexcept;
};
} // namespace Tourmaline::Game::Input

#endif
