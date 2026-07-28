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

#include "../Systems/ECS.hpp"

#include "Corrade/Containers/String.h"
#include "Magnum/Magnum.h"
#include "Magnum/Platform/GlfwApplication.h"
#include "Magnum/Tags.h"

namespace Tourmaline {
using namespace Magnum::Math::Literals;

class GameProgram : private Magnum::Platform::GlfwApplication {
public:
  struct Config {
    Corrade::Containers::String windowTitle{"Game Window"};
    Magnum::Vector2i windowSize{800, 600};
    uint64_t desiredFrameRate = 60;
  };

  explicit GameProgram()
      : Magnum::Platform::Application{Arguments{argc, _argv},
                                      Magnum::NoCreate} {};
  GameProgram(GameProgram &&) = delete;
  GameProgram(const GameProgram &) = delete;
  int Run(const Config &conf);

  virtual void OnStart();
  virtual void OnStep();
  virtual void OnExit();
  Systems::ECS::World ECS;

private:
  void initialize();
  void drawEvent() override;
  Config config;

  // Magnum forcing me to use Arguments -_-
  inline static int argc = 1;
  inline static char _argv0[] = "empty";
  inline static char *_argv[] = {_argv0};
};
} // namespace Tourmaline
#endif
