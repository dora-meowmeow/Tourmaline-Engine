/*
 * SPDX-FileCopyrightText: Dora "cat" <cat@thenight.club>
 * SPDX-License-Identifier: MPL-2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "Game/Input/Keyboard.hpp"

using namespace Tourmaline::Game::Input;

bool Key::IsInactive() const noexcept { return state == KeyState::Inactive; }
bool Key::IsActive() const noexcept { return state != KeyState::Inactive; }
bool Key::IsDown() const noexcept {
  return state == KeyState::Pressed || state == KeyState::Held;
}
bool Key::IsPressed() const noexcept { return state == KeyState::Pressed; }
bool Key::IsReleased() const noexcept { return state == KeyState::Released; }
bool Key::IsHeld() const noexcept { return state == KeyState::Held; }
