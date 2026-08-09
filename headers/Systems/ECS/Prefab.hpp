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

/**
 * @file
 * @brief Basic Prefab capacity.
 */

namespace Tourmaline::Systems::ECS {

/**
 * @brief The prefab class. Prefabs are used to quicky mass produce
 * entities with multiple pre-determined components.
 * @tparam Components Any classes that satisfies
 * Tourmaline::Systems::Ecs::isAcomponent concept.
 */
template <isAComponent... Components> class Prefab {
private:
  std::tuple<Components...> components;

public:
  using tupleSignature = decltype(components);

  /**
   * @brief Constructs a prefab with given arguments.
   * @param arguments These arguments are forwarded to construct each component
   * inside the prefab.
   */
  Prefab(Components... arguments)
      : components(std::forward_as_tuple(arguments...)) {}

  /**
   * @brief Interal tuple storage of each component.
   * @return A reference to the interal tuple.
   */
  std::tuple<Components...> &GetTuple() { return components; }

  /**
   * @brief Interal tuple storage of each component.
   * @tparam Component any classes that satisfies
   * Tourmaline::Systems::Ecs::isAcomponent concept.
   * Must be a component type that is already inside this prefab.
   * @return A reference to the requested component.
   */
  template <isAComponent Component> Component &GetComponent() {
    return std::get<Component>(components);
  };
};

} // namespace Tourmaline::Systems::ECS
#endif
