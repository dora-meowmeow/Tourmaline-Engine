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

namespace Tourmaline::Game::Input {
enum class KeyState {
  Inactive = 0,
  Pressed = 1,
  Released = 2,
  Held = 3,
};

using KeyType = Magnum::Platform::GlfwApplication::Key;

class Key {
public:
  KeyState state;
  KeyType type;

  bool IsInactive() const noexcept;
  bool IsActive() const noexcept;
  bool IsDown() const noexcept;
  bool IsPressed() const noexcept;
  bool IsReleased() const noexcept;
  bool IsHeld() const noexcept;
};
} // namespace Tourmaline::Game::Input

#endif
