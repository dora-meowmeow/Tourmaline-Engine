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

/**
 * @file
 * @brief Builtin components and Base component.
 */

namespace Tourmaline::Systems::ECS {

/// @brief Base component that every component must publically inherit from.
struct Component {
public:
  /**
   * @brief Whether or not if the component is enabled.
   */
  bool isEnabled;

  /// @brief Default constructor for components.
  Component(bool enabled = true) : isEnabled(enabled) {};
};

/**
 * @brief Check if a type is a valid component.
 * @tparam T type to check if it inherits from
 * Tourmaline::Systems::ECS::Component.
 */
template <typename T>
concept isAComponent = std::derived_from<T, ECS::Component>;
} // namespace Tourmaline::Systems::ECS

namespace Tourmaline::Systems::Components {
/**
 * @brief Fundamental Transform component that every entity in ECS must have.
 */
struct Transform : public ECS::Component {
  /**
   * @brief A 3 dimentional vector using doubles. Used to set position of the
   * entity.
   */
  Magnum::Math::Vector3<double> Translate{0};
  /**
   * @brief A 3 dimentional vector using doubles. Used to set rotation of the
   * entity.
   */
  Magnum::Math::Vector3<double> Rotate{0};
  /**
   * @brief A 3 dimentional vector using doubles. Used to set scale of the
   * entity.
   */
  Magnum::Math::Vector3<double> Scale{1};
};
} // namespace Tourmaline::Systems::Components
#endif
