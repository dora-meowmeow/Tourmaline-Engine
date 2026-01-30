/*
 * SPDX-FileCopyrightText: Dora "cat" <cat@thenight.club>
 * SPDX-License-Identifier: MPL-2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef GUARD_TOURMALINE_BUILTIN_COMPONENTS_H
#define GUARD_TOURMALINE_BUILTIN_COMPONENTS_H

#include <concepts>

namespace Tourmaline::Systems::ECS {
class World;
}

namespace Tourmaline::Systems::Components {
// Base
struct BaseComponent {
public:
  virtual ~BaseComponent() = default;

private:
  friend class World;
};

template <typename T>
concept Component = std::derived_from<T, BaseComponent>;

// Builtin
struct Position : public BaseComponent {
  Position(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}
  double x, y, z;
};

struct Enabled : public BaseComponent {
  Enabled(Tourmaline::Systems::ECS::World *world) : ownerWorld(world) {}

  [[nodiscard]]
  bool isEnabled();
  void setEnabled(bool enable = true);

private:
  bool enabled = true;
  Tourmaline::Systems::ECS::World *ownerWorld;
};
} // namespace Tourmaline::Systems::Components
#endif
