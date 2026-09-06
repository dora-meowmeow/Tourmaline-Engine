/*
 * SPDX-FileCopyrightText: Dora "cat" <cat@thenight.club>
 * SPDX-License-Identifier: MPL-2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "Game/Program.hpp"

#include "Magnum/GL/AbstractFramebuffer.h"
#include "Magnum/GL/DefaultFramebuffer.h"
#include "Magnum/Math/Time.h"
#include "Magnum/Platform/GlfwApplication.h"

using namespace Magnum;
using namespace Tourmaline::Game;

Program::Args Program::arguments{Program::_argc, &Program::_argv};

void Program::OnStart() {}
void Program::OnStep() {}
bool Program::OnExit() { return true; }

void Program::initialize() {
  ApplyNewConfig();
  create(magnumConfig);
  isWindowCreated = true;
}

int Program::Run(const Config &conf) {
  config = conf;
  initialize();
  OnStart();
  timeline.start();
  return exec();
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
  if (isWindowCreated) {
    updateWindowSettings(magnumConfig);
  }
}

void Program::drawEvent() {
  GL::defaultFramebuffer.clear(GL::FramebufferClear::Color |
                               GL::FramebufferClear::Depth);
  OnStep();
  ECS.Step();

  swapBuffers();
  redraw();

  timeline.nextFrame();
  deltaTime = timeline.previousFrameDuration();
}

void Program::exitEvent(ExitEvent &event) { event.setAccepted(OnExit()); }
