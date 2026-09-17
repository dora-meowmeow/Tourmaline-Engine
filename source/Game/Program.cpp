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
#include "Magnum/Math/Time.h"
#include "Magnum/Platform/GlfwApplication.h"

using namespace Magnum;
using namespace Tourmaline::Game;
using namespace Tourmaline::Game::Input;

Program::Args Program::arguments{Program::_argc, &Program::_argv};

void Program::OnStart() {}
void Program::OnStep() {}
bool Program::OnExit() { return true; }

const Input::Key &Program::GetKey(const KeyType &keyType) {
  if (!_inputTrack.Has(keyType)) {
    _inputTrack.Insert(keyType, {KeyState::Inactive, keyType});
  }

  return _inputTrack.Get(keyType);
}

void Program::initialize() { ApplyNewConfig(); }

void Program::advanceKeyEvents() {
  if (_advanceKeys.size() == 0) {
    return;
  }

  for (const KeyType &keyType : _advanceKeys) {
    Input::Key &key = _inputTrack.Get(keyType);
    key.state =
        key.state == KeyState::Pressed ? KeyState::Held : KeyState::Inactive;
  }
  _advanceKeys.clear();
}

int Program::Run(const Config &conf) {
  config = conf;
  initialize();
  OnStart();
  timeline.start();
  return mainLoopIteration();
}

int Program::Run() { return mainLoopIteration(); }

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
  ECS.Step();

  swapBuffers();
  redraw();

  advanceKeyEvents();
  timeline.nextFrame();
  _deltaTime = timeline.previousFrameDuration();
}

void Program::viewportEvent(ViewportEvent &event) {
  GL::defaultFramebuffer.setViewport({{}, event.framebufferSize()});
}

void Program::keyPressEvent(KeyEvent &event) {
  KeyType type = event.key();
  _advanceKeys.push_back(type);

  if (!_inputTrack.Has(type)) {
    _inputTrack.Insert(type, {KeyState::Pressed, type});
    return;
  }

  _inputTrack.Get(type).state = KeyState::Pressed;
}

void Program::keyReleaseEvent(KeyEvent &event) {
  KeyType type = event.key();
  _advanceKeys.push_back(type);

  // It is possible to get a release event before a press event!
  if (!_inputTrack.Has(type)) {
    _inputTrack.Insert(type, {KeyState::Released, type});
    return;
  }

  _inputTrack.Get(type).state = KeyState::Released;
}

void Program::exitEvent(ExitEvent &event) { event.setAccepted(OnExit()); }
