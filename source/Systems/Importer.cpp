/*
 * SPDX-FileCopyrightText: Dora "cat" <cat@thenight.club>
 * SPDX-License-Identifier: MPL-2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "Systems/Importer.hpp"
#include "Magnum/GL/GL.h"
#include "Magnum/GL/Sampler.h"
#include "Magnum/Trade/Trade.h"
#include "Systems/Logging.hpp"

#include "Corrade/Containers/Optional.h"

#include "Magnum/GL/Texture.h"
#include "Magnum/GL/TextureFormat.h"
#include "Magnum/ImageView.h"
#include "Magnum/MeshTools/Compile.h"
#include "Magnum/Trade/MeshData.h"

using namespace Tourmaline::Systems;
using namespace Magnum;
using namespace Corrade;

PluginManager::Manager<Trade::AbstractImporter> Importer::pluginManager;

Containers::Pointer<Trade::AbstractImporter> Importer::imageImporter =
    pluginManager.loadAndInstantiate("AnyImageImporter");

Containers::Pointer<Trade::AbstractImporter> Importer::sceneImporter =
    pluginManager.loadAndInstantiate("AnySceneImporter");

Trade::ImageData2D Importer::LoadImage2D(Containers::StringView path) {
  Logging::Log("AnyImageImporter plugin failed to be loaded. Terminating...",
               "Importer/LoadImage2D", Logging::Critical, !imageImporter);

  if (!imageImporter->openFile(path)) {
    Logging::LogFormatted("Could not open file {}! Throwing...",
                          "Importer/LoadImage2D", Logging::Error, path);
  }

  return *imageImporter->image2D(0);
}

GL::Texture2D Importer::LoadTexture2D(Trade::ImageData2D image,
                                      GL::SamplerFilter filterType,
                                      GL::SamplerWrapping wrappingRule) {
  GL::Texture2D texture;
  texture.setWrapping(wrappingRule)
      .setMagnificationFilter(filterType)
      .setMinificationFilter(filterType)
      .setStorage(1, GL::textureFormat(image.format()), image.size())
      .setSubImage(0, {}, image)
      .generateMipmap();

  return texture;
}

GL::Texture2D Importer::LoadTexture2D(Containers::StringView path,
                                      GL::SamplerFilter filterType,
                                      GL::SamplerWrapping wrappingRule) {
  return LoadTexture2D(LoadImage2D(path), filterType, wrappingRule);
}

GL::Mesh Importer::LoadObject(Containers::StringView path) {
  Logging::Log("AnySceneImporter plugin failed to be loaded. Terminating...",
               "Importer/LoadObject", Logging::Critical, !sceneImporter);

  if (!sceneImporter->openFile(path)) {
    Logging::LogFormatted("Could not open file {}! Throwing...",
                          "Importer/LoadObject", Logging::Error, path);
  }

  return MeshTools::compile(*sceneImporter->mesh(0));
}
