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

#include "Magnum/Math/Vector3.h"
#include <concepts>

namespace Tourmaline::Systems::ECS {
struct Component {
public:
  virtual ~Component() = default;
};
template <typename T>
concept isAComponent = std::derived_from<T, ECS::Component>;
} // namespace Tourmaline::Systems::ECS

namespace Tourmaline::Systems::Components {
// Builtin
struct Transform : public ECS::Component {
  Magnum::Math::Vector3<double> Translate{0};
  Magnum::Math::Vector3<double> Rotate{0};
  Magnum::Math::Vector3<double> Scale{1};
};
} // namespace Tourmaline::Systems::Components
#endif
