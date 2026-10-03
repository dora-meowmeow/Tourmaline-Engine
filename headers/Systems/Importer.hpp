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
#include <vector>

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
 * It is heavily suggested that you [familiarise yourself with how Magnum
 * Graphics works.](https://doc.magnum.graphics/magnum/)
 *
 * @see [Magnum Graphics' Magnum::GL namespace
 * documentation](https://doc.magnum.graphics/magnum/namespaceMagnum_1_1GL.html)
 */
class Importer {
public:
  /**
   * @brief Info about the mesh, and the mesh data itself.
   */
  struct MeshInfo {
    uint64_t index;
    Corrade::Containers::String name;
    Magnum::GL::Mesh mesh;
  };

  /**
   * @brief Imports an image asset as an ImageData2D.
   *
   * @param path Path to the asset. See @ref supportedFileTypes to learn
   * which file-types are supported.
   *
   * @return The image asset imported as an ImageData2D.
   *
   * @warning If this function fails to find the specified asset at the given
   * path, it will throw!
   *
   * @see Magnum::Trade::ImageData
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
   * @see Magnum::Trade::ImageData
   * @see Magnum::GL::SamplerFilter
   * @see Magnum::GL::SamplerWrapping
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
   * @warning If this function fails to find the specified asset at the given
   * path, it will throw!
   *
   * @see Magnum::GL::Texture
   * @see Magnum::GL::SamplerFilter
   * @see Magnum::GL::SamplerWrapping
   */
  [[nodiscard("Unnecesary call to LoadTexture2D")]]
  static Magnum::GL::Texture2D LoadTexture2D(
      Corrade::Containers::StringView path,
      Magnum::GL::SamplerFilter filterType = Magnum::GL::SamplerFilter::Nearest,
      Magnum::GL::SamplerWrapping wrappingRule =
          Magnum::GL::SamplerWrapping::ClampToEdge);

  /**
   * @brief Imports an 3D Object's mesh as a GL::Mesh.
   *
   * @param path Path to the asset. See @ref supportedFileTypes to learn
   * which file-types are supported.
   * @param name Some 3D file-types support multiple meshes in same file. You
   * can select which object you want by setting this value.
   *
   * @return The 3D Object's mesh as a GL::Mesh.
   *
   * @warning If this function fails to find the specified asset at the given
   * path, it will throw!
   *
   * @see Magnum::GL::Mesh
   */
  [[nodiscard("Unnecesary call to LoadMesh")]]
  static Magnum::GL::Mesh LoadMesh(Corrade::Containers::StringView path,
                                   uint64_t index = 0);

  /**
   * @brief Imports an 3D Object's mesh as a GL::Mesh.
   *
   * @param path Path to the asset. See @ref supportedFileTypes to learn
   * which file-types are supported.
   * @param name Some 3D file-types support multiple meshes in same file. You
   * can select which object you want by setting this value.
   *
   * @return The 3D Object's mesh as a GL::Mesh.
   *
   * @warning If this function fails to find the specified asset at the given
   * path, it will throw!
   *
   * @see Magnum::GL::Mesh
   */
  [[nodiscard("Unnecesary call to LoadMesh")]]
  static Magnum::GL::Mesh LoadMesh(Corrade::Containers::StringView path,
                                   Corrade::Containers::StringView name);

  /**
   * @brief Imports all of the meshes in a 3D assets as a GL::Mesh.
   *
   * @param path Path to the asset. See @ref supportedFileTypes to learn
   * which file-types are supported.
   *
   * @return An array containing the 3D assets' meshes as
   * Tourmaline::Systems::Importer::MeshInfo.
   *
   * @warning If this function fails to find the specified asset at the given
   * path, it will throw!
   *
   * @see Magnum::GL::Mesh
   */
  [[nodiscard("Unnecesary call to LoadAllMeshes")]]
  static std::vector<MeshInfo>
  LoadAllMeshes(Corrade::Containers::StringView path);

private:
  static Corrade::PluginManager::Manager<Magnum::Trade::AbstractImporter>
      pluginManager;
  static Corrade::Containers::Pointer<Magnum::Trade::AbstractImporter>
      imageImporter, sceneImporter;
};

} // namespace Tourmaline::Systems
#endif
