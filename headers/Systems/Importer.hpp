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
#include "Magnum/GL/Texture.h"
#include "Magnum/Trade/AbstractImporter.h"
#include "Magnum/Trade/ImageData.h"

/**
 * @file
 * @brief Asset importing related features
 */

namespace Tourmaline::Systems {
/**
 * @brief A static class that simplifies importing assets.
 *
 * Assets used in Tourmaline Engine are stored in Magnum Graphics' own classes
 * (Specifically Magnum::GL namespace). This is due to the backend rendering
 * being mostly done by Magnum Graphics.
 *
 * It is heavily suggested that you [get yourself familiarised with how Magnum
 * Graphics works.](https://doc.magnum.graphics/magnum/)
 *
 * @see [Magnum Graphics' Magnum::GL namespace
 * documentation](https://doc.magnum.graphics/magnum/namespaceMagnum_1_1GL.html)
 */
class Importer {
public:
  /**
   * @brief Imports an image asset as a ImageData2D.
   *
   * @param path Path to the asset. See @ref supportedFileTypes to learn
   * which file-types are supported.
   *
   * @return The image asset imported as an ImageData2D.
   *
   * @warning If this function fails to find the specified asset at given path,
   * it will throw!
   *
   * @see [Magnum::Trade::ImageData](
   * https://doc.magnum.graphics/magnum/classMagnum_1_1Trade_1_1ImageData.html)
   */
  [[nodiscard("Unnecesary call to LoadImage2D")]]
  static Magnum::Trade::ImageData2D
  LoadImage2D(Corrade::Containers::StringView path);

  /**
   * @brief Converts an ImageData2D to Texture2D.
   *
   * @param image The image data to use.
   * @param filterType The type of texture filtering to prefer.
   * @param wrappingRule The way the image should be wrapped.
   *
   * @return The image data as a Texture2D.
   *
   * @see [Magnum::Trade::ImageData](
   * https://doc.magnum.graphics/magnum/classMagnum_1_1Trade_1_1ImageData.html)
   * @see [Magnum::GL::SamplerFilter](
   * https://doc.magnum.graphics/magnum/namespaceMagnum_1_1GL.html#ac5625b1b6d9959c520df40ccaca38f70)
   * @see [Magnum::GL::SamplerWrapping](
   * https://doc.magnum.graphics/magnum/namespaceMagnum_1_1GL.html#ae807f5b816812ec007253cae4ddc4520)
   */
  [[nodiscard("Unnecesary call to LoadTexture2D")]]
  static Magnum::GL::Texture2D LoadTexture2D(
      Magnum::Trade::ImageData2D image,
      Magnum::GL::SamplerFilter filterType = Magnum::GL::SamplerFilter::Nearest,
      Magnum::GL::SamplerWrapping wrappingRule =
          Magnum::GL::SamplerWrapping::ClampToEdge);

  /**
   * @brief Imports an image asset as a Texture2D.
   *
   * @param path Path to the asset. See @ref supportedFileTypes to learn
   * which file-types are supported.
   * @param filterType The type of texture filtering to prefer.
   * @param wrappingRule The way the image should be wrapped.
   *
   * @return The image imported as a Texture2D.
   *
   * @warning If this function fails to find the specified asset at given path,
   * it will throw!
   *
   * @see [Magnum::GL::Texture](
   * https://doc.magnum.graphics/magnum/classMagnum_1_1GL_1_1Texture.html)
   * @see [Magnum::GL::SamplerFilter](
   * https://doc.magnum.graphics/magnum/namespaceMagnum_1_1GL.html#ac5625b1b6d9959c520df40ccaca38f70)
   * @see [Magnum::GL::SamplerWrapping](
   * https://doc.magnum.graphics/magnum/namespaceMagnum_1_1GL.html#ae807f5b816812ec007253cae4ddc4520)
   */
  [[nodiscard("Unnecesary call to LoadTexture2D")]]
  static Magnum::GL::Texture2D LoadTexture2D(
      Corrade::Containers::StringView path,
      Magnum::GL::SamplerFilter filterType = Magnum::GL::SamplerFilter::Nearest,
      Magnum::GL::SamplerWrapping wrappingRule =
          Magnum::GL::SamplerWrapping::ClampToEdge);

  /**
   * @brief Imports an 3D asset as a GL::Mesh.
   *
   * @param path Path to the asset. See @ref supportedFileTypes to learn
   * which file-types are supported.
   *
   * @return The 3D asset as a GL::Mesh.
   *
   * @warning If this function fails to find the specified asset at given path,
   * it will throw!
   *
   * @see [Magnum::GL::Mesh](
   * https://doc.magnum.graphics/magnum/classMagnum_1_1GL_1_1Mesh.html)
   */
  [[nodiscard("Unnecesary call to LoadObject")]]
  static Magnum::GL::Mesh LoadObject(Corrade::Containers::StringView path);

private:
  static Corrade::PluginManager::Manager<Magnum::Trade::AbstractImporter>
      pluginManager;
  static Corrade::Containers::Pointer<Magnum::Trade::AbstractImporter>
      imageImporter, sceneImporter;
};

} // namespace Tourmaline::Systems
#endif
