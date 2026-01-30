/*
 * SPDX-FileCopyrightText: Dora "cat" <cat@thenight.club>
 * SPDX-License-Identifier: MPL-2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <Systems/ECS.hpp>
#include <Systems/ECS/BuiltinComponents.hpp>
#include <utility>

bool Tourmaline::Systems::Components::Enabled::isEnabled() { return enabled; }
void Tourmaline::Systems::Components::Enabled::setEnabled(bool enable) {
  ownerWorld->entitiesToDisable.emplace(std::pair{this, enable});
}
