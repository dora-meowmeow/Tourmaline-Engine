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

using namespace Magnum;

void Tourmaline::Game::Program::OnStart() {}
void Tourmaline::Game::Program::OnStep() {}
void Tourmaline::Game::Program::OnExit() {}

void Tourmaline::Game::Program::initialize() {
  create(
      Configuration{}.setTitle(config.windowTitle).setSize(config.windowSize));
  setMinimalLoopPeriod(static_cast<Magnum::Nanoseconds>(1.0_sec) /
                       config.desiredFrameRate);
}

int Tourmaline::Game::Program::Run(const Config &conf) {
  config = conf;
  initialize();
  OnStart();
  return exec();
}

void Tourmaline::Game::Program::drawEvent() {
  GL::defaultFramebuffer.clear(GL::FramebufferClear::Color |
                               GL::FramebufferClear::Depth);
  OnStep();
  ECS.Step();
  swapBuffers();
  redraw();
}
