/*
 * SPDX-FileCopyrightText: Dora "cat" <cat@thenight.club>
 * SPDX-License-Identifier: MPL-2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef GUARD_TOURMALINE_PREFAB_H
#define GUARD_TOURMALINE_PREFAB_H

#include "BuiltinComponents.hpp"

#include <tuple>

namespace Tourmaline::Systems::ECS {
template <isAComponent... Components> class Prefab {
private:
  std::tuple<Components...> components;

public:
  using tupleSignature = decltype(components);

  Prefab(Components... arguments)
      : components(std::forward_as_tuple(arguments...)) {}

  std::tuple<Components...> &GetTuple() { return components; }

  template <isAComponent Component> Component &GetComponent() {
    return std::get<Component>(components);
  };
};

} // namespace Tourmaline::Systems::ECS
#endif
