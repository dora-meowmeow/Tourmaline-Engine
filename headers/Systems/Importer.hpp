/*
 * SPDX-FileCopyrightText: Dora "cat" <cat@thenight.club>
 * SPDX-License-Identifier: MPL-2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at http://mozilla.org/MPL/2.0/.
 */
#ifndef GUARD_TOURMALINE_IMPORTER_H
#define GUARD_TOURMALINE_IMPORTER_H

#include "Corrade/Containers/Pointer.h"
#include "Corrade/Containers/StringView.h"
#include "Corrade/PluginManager/PluginManager.h"

#include "Magnum/GL/GL.h"
#include "Magnum/GL/Mesh.h"
#include "Magnum/GL/Sampler.h"
#include "Magnum/Trade/AbstractImporter.h"
#include "Magnum/Trade/ImageData.h"

namespace Tourmaline::Systems {

class Importer {
public:
  static Magnum::Trade::ImageData2D
  LoadImage2D(Corrade::Containers::StringView path);
  static Magnum::GL::Texture2D LoadTexture2D(
      Magnum::Trade::ImageData2D image,
      Magnum::GL::SamplerFilter filterType = Magnum::GL::SamplerFilter::Nearest,
      Magnum::GL::SamplerWrapping wrappingRule =
          Magnum::GL::SamplerWrapping::ClampToEdge);

  static Magnum::GL::Texture2D LoadTexture2D(
      Corrade::Containers::StringView path,
      Magnum::GL::SamplerFilter filterType = Magnum::GL::SamplerFilter::Nearest,
      Magnum::GL::SamplerWrapping wrappingRule =
          Magnum::GL::SamplerWrapping::ClampToEdge);
  static Magnum::GL::Mesh LoadObject(Corrade::Containers::StringView path);

private:
  static Corrade::PluginManager::Manager<Magnum::Trade::AbstractImporter>
      pluginManager;
  static Corrade::Containers::Pointer<Magnum::Trade::AbstractImporter>
      imageImporter, sceneImporter;
};

} // namespace Tourmaline::Systems
#endif
