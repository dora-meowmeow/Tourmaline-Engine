/*
 * SPDX-FileCopyrightText: Dora "cat" <cat@thenight.club>
 * SPDX-License-Identifier: MPL-2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "Game/Program.hpp"
#include "Game/Input.hpp"

#include "Magnum/GL/AbstractFramebuffer.h"
#include "Magnum/GL/DefaultFramebuffer.h"
#include "Magnum/Magnum.h"
#include "Magnum/Math/Functions.h"
#include "Magnum/Math/Time.h"
#include "Magnum/Platform/GlfwApplication.h"

#include <exception>

using namespace Magnum;
using namespace Tourmaline::Game;
using namespace Tourmaline::Game::Input;

Program::Args Program::arguments{Program::_argc, &Program::_argv};

void Program::OnStart() {}
void Program::OnStep() {}
void Program::OnCrash(const std::exception &e) { throw e; }
bool Program::OnExit() { return true; }

const Input::Key &Program::GetKey(const KeyType &keyType) {
  if (!_keyboardTrack.Has(keyType)) {
    _keyboardTrack.Insert(keyType, {KeyState::Inactive, keyType});
  }

  return _keyboardTrack.Get(keyType);
}

void Program::initialize() { ApplyNewConfig(); }

void Program::advanceInputEvents() {
  if (_advanceKeys.size()) {
    for (const KeyType &keyType : _advanceKeys) {
      Input::Key &key = _keyboardTrack.Get(keyType);
      key.state =
          key.state == KeyState::Pressed ? KeyState::Held : KeyState::Inactive;
    }
    _advanceKeys.clear();
  }

  if (_advancePointerButtons.size()) {
    Input::Pointer mouse = GetMouse();
    for (PointerButtonState *buttonState : _advancePointerButtons) {
      *(buttonState) = *buttonState == PointerButtonState::Pressed
                           ? PointerButtonState::Held
                           : PointerButtonState::Inactive;
    }
    _advancePointerButtons.clear();
  }
  _primaryPointer.scrollAmount = {0.0f, 0.0f};
}

int Program::Run(const Config &conf) {
  config = conf;
  try {
    initialize();
    OnStart();
    timeline.start();
    return exec();
  } catch (const std::exception &e) {
    OnCrash(e);
  }

  return 1;
}

void Program::ApplyNewConfig() {
  magnumConfig.setTitle(config.windowTitle)
      .setSize(config.windowSize)
      .setWindowFlags(
          static_cast<Configuration::WindowFlag>(config.windowMode));

  if (config.desiredFrameRate != 0) {
    setMinimalLoopPeriod(1.0_sec / config.desiredFrameRate);
  }
  setSwapInterval(config.vsyncEnabled);

  // Only available on glfw (for now)
  updateWindowSettings(magnumConfig);

  _aspectRatio =
      static_cast<float>(config.windowSize.x()) / config.windowSize.y();
}

void Program::drawEvent() {
  GL::defaultFramebuffer.clear(GL::FramebufferClear::Color |
                               GL::FramebufferClear::Depth);
  OnStep();
  World.Step();

  swapBuffers();
  redraw();

  advanceInputEvents();
  timeline.nextFrame();
  _deltaTime = timeline.previousFrameDuration();
}

void Program::viewportEvent(ViewportEvent &event) {
  GL::defaultFramebuffer.setViewport({{}, event.framebufferSize()});
}

void Program::keyPressEvent(KeyEvent &event) {
  KeyType type = event.key();
  _advanceKeys.push_back(type);

  if (!_keyboardTrack.Has(type)) {
    _keyboardTrack.Insert(type, {KeyState::Pressed, type});
    return;
  }

  _keyboardTrack.Get(type).state = KeyState::Pressed;
}

void Program::keyReleaseEvent(KeyEvent &event) {
  KeyType type = event.key();
  _advanceKeys.push_back(type);

  // It is possible to get a release event before a press event!
  if (!_keyboardTrack.Has(type)) {
    _keyboardTrack.Insert(type, {KeyState::Released, type});
    return;
  }

  _keyboardTrack.Get(type).state = KeyState::Released;
}

void Program::exitEvent(ExitEvent &event) { event.setAccepted(OnExit()); }

const Input::Pointer &Program::GetMouse() { return _primaryPointer; }

void Program::pointerPressEvent(PointerEvent &event) {
  // Do not use GetButtonState because that returns a const
  Input::PointerButtonState &state =
      _primaryPointer.buttonStates.Get(event.pointer());
  state = PointerButtonState::Pressed;
  _advancePointerButtons.push_back(&state);
}

void Program::pointerReleaseEvent(PointerEvent &event) {
  // Do not use GetButtonState because that returns a const
  Input::PointerButtonState &state =
      _primaryPointer.buttonStates.Get(event.pointer());
  state = PointerButtonState::Released;
  _advancePointerButtons.push_back(&state);
}

void Program::pointerMoveEvent(PointerMoveEvent &event) {
  _primaryPointer.position =
      static_cast<Vector2i>(Magnum::Math::round(event.position()));
}

void Program::scrollEvent(ScrollEvent &event) {
  _primaryPointer.scrollAmount = event.offset();
}
