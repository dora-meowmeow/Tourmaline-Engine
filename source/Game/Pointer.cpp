/*
 * SPDX-FileCopyrightText: Dora "cat" <cat@thenight.club>
 * SPDX-License-Identifier: MPL-2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "Game/Input/Pointer.hpp"

using namespace Tourmaline;
using namespace Tourmaline::Game::Input;

decltype(Pointer::buttonStates) Pointer::preparedButtonStates{};

PointerButtonState
Pointer::GetButtonState(PointerButtonType button) const noexcept {
  return buttonStates.GetView(button);
}

bool Pointer::IsInactive(PointerButtonType button) const noexcept {
  return GetButtonState(button) == PointerButtonState::Inactive;
}
bool Pointer::IsActive(PointerButtonType button) const noexcept {
  return GetButtonState(button) != PointerButtonState::Inactive;
}
bool Pointer::IsDown(PointerButtonType button) const noexcept {
  PointerButtonState state = GetButtonState(button);
  return state == PointerButtonState::Pressed ||
         state == PointerButtonState::Held;
}
bool Pointer::IsPressed(PointerButtonType button) const noexcept {
  return GetButtonState(button) == PointerButtonState::Pressed;
}
bool Pointer::IsReleased(PointerButtonType button) const noexcept {
  return GetButtonState(button) == PointerButtonState::Released;
}
bool Pointer::IsHeld(PointerButtonType button) const noexcept {
  return GetButtonState(button) == PointerButtonState::Held;
}
