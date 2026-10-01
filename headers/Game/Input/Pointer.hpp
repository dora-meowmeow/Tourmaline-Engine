/*
 * SPDX-FileCopyrightText: Dora "cat" <cat@thenight.club>
 * SPDX-License-Identifier: MPL-2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef GUARD_TOURMALINE_INPUT_POINTER_H
#define GUARD_TOURMALINE_INPUT_POINTER_H

#include "../../Containers/Hashmap.hpp"
#include "Magnum/Platform/GlfwApplication.h"
/**
 * @file
 * @brief Pointer/Mouse input related types
 */
namespace Tourmaline::Game {
class Program;
}

namespace Tourmaline::Game::Input {
/**
 * @brief The states a pointer's (like a mouse) button can be in.
 *
 * @note A released event may be triggered without a prior pressed event
 * triggering. This usually occurs when the program window is brought into focus
 * while a mouse button is held down. This rarely causes an issue, but should
 * still be noted.
 */
enum class PointerButtonState {
  Inactive = 0,
  Pressed = 1,
  Released = 2,
  Held = 3,
};

/**
 * @brief The pointer button codes that GLFW uses.
 *
 * @see
 * [Magnum::Platform::GlfwApplication::Pointer](https://doc.magnum.graphics/magnum/classMagnum_1_1Platform_1_1GlfwApplication.html#aa40e11a0a9664b881e6ce916aeea72c4)
 */
using PointerButtonType = Magnum::Platform::GlfwApplication::Pointer;

struct Pointer {
  Pointer() {
    if (!preparedButtonStates.Count()) {
      for (PointerButtonType x :
           {PointerButtonType::MouseLeft, PointerButtonType::MouseRight,
            PointerButtonType::MouseMiddle, PointerButtonType::MouseButton4,
            PointerButtonType::MouseButton5, PointerButtonType::MouseButton6,
            PointerButtonType::MouseButton7, PointerButtonType::MouseButton8}) {
        preparedButtonStates.Insert(x, PointerButtonState::Inactive);
      }
    }
    buttonStates = preparedButtonStates;
  }
  /**
   * @brief The position of the mouse on the screen. Top left is 0,0.
   */
  Magnum::Vector2i position;

  /**
   * @brief The distance that is scrolled.
   *
   * On X axis: scrolling LEFT is considered positive,
   * while scrolling RIGHT is negative distance.
   *
   * On Y axis: scrolling UP is considered positive,
   * while scrolling DOWN is negative distance.
   */
  Magnum::Vector2 scrollAmount;

  /**
   * @brief Returns the state of a pointer button.
   *
   * @return const reference to
   * Tourmaline::Game::Input::Pointer::PointerButtonState of the button.
   */
  PointerButtonState GetButtonState(PointerButtonType button) const noexcept;

  /**
   * @brief Quality of Life function to check state.
   *
   * @return Returns true if state is equal to
   * [PointerButtonState::Inactive](@ref Tourmaline::Game::Input::PointerButtonState), 
   * otherwise returns false.
   */
  bool IsInactive(PointerButtonType button) const noexcept;

  /**
   * @brief Quality of Life function to check state.
   *
   * @return Returns true if state __**IS NOT**__ equal to
   * [PointerButtonState::Inactive](@ref Tourmaline::Game::Input::PointerButtonState), 
   * otherwise returns false.
   */
  bool IsActive(PointerButtonType button) const noexcept;

  /**
   * @brief Quality of Life function to check state.
   *
   * @return Returns true if state is equal to either
   * [PointerButtonState::Pressed](@ref Tourmaline::Game::Input::PointerButtonState)
   * or
   * [PointerButtonState::Held](@ref Tourmaline::Game::Input::PointerButtonState), 
   * otherwise returns false.
   */
  bool IsDown(PointerButtonType button) const noexcept;

  /**
   * @brief Quality of Life function to check state.
   *
   * @return Returns true if state is equal to
   * [PointerButtonState::Pressed](@ref Tourmaline::Game::Input::PointerButtonState),
   * otherwise returns false.
   */
  bool IsPressed(PointerButtonType button) const noexcept;

  /**
   * @brief Quality of Life function to check state.
   *
   * @return Returns true if state is equal to
   * [PointerButtonState::Released](@ref Tourmaline::Game::Input::PointerButtonState), 
   * otherwise returns false.
   */
  bool IsReleased(PointerButtonType button) const noexcept;

  /**
   * @brief Quality of Life function to check state.
   *
   * @return Returns true if state is equal to
   * [PointerButtonState::Held](@ref Tourmaline::Game::Input::PointerButtonState), 
   * otherwise returns false.
   */
  bool IsHeld(PointerButtonType button) const noexcept;

private:
  Containers::Hashmap<PointerButtonType, PointerButtonState> buttonStates;
  static decltype(buttonStates) preparedButtonStates;
  friend class Tourmaline::Game::Program;
};

} // namespace Tourmaline::Game::Input
#endif
