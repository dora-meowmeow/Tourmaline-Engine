/*
 * SPDX-FileCopyrightText: Dora "cat" <cat@thenight.club>
 * SPDX-License-Identifier: MPL-2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "../ECS.hpp"

namespace Tourmaline::Systems::Components {
struct Position : public Tourmaline::Systems::ECS::BaseComponent {
  Position(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}
  double x, y, z;
};
} // namespace Tourmaline::Systems::Components
