\page supportedFileTypes Supported File-types

# How does Tourmaline import assets?

Tourmaline Engine heavily relies on [Magnum Graphics' importer plugins](https://github.com/mosra/magnum-plugins) under the hood. This is by design, as Magnum Graphics is the graphics back end for Tourmaline Engine. It is heavily suggested that you [get yourself familiarised on how Magnum Graphics and it's importers work!](https://doc.magnum.graphics/magnum/) You can check out Tourmaline::Systems::Importer to learn more about how to import assets.

Currently the following importers are compiled when Tourmaline Engine compiles Magnum Graphics as a dependency (This list is not final and may grow/shrink/change overtime):

## Images/Textures

- [StbiImporter](https://doc.magnum.graphics/magnum/classMagnum_1_1Trade_1_1StbImageImporter.html) adds capabilities of [stb_image](https://github.com/nothings/stb). Supports:
  - `*.bmp`
  - `*.gif`
  - `*.hdr`
  - `*.jpg/jpe/jpeg`
  - `*.pgm`
  - `*.pic`
  - `*.png`
  - `*.ppm`
  - `*.psd`
  - `*.tga/vda/icb/vst`

## 3D

- [UfbxImporter](https://doc.magnum.graphics/magnum/classMagnum_1_1Trade_1_1UfbxImporter.html) supports:
  - `*.obj`
  - `*.fbx`
- [GltfImporter](https://doc.magnum.graphics/magnum/classMagnum_1_1Trade_1_1GltfImporter.html) supports:
  - `.gltf/glb`

@note Tourmaline Engine also compiles AnyImageImporter and AnySceneImporter for ease of use.
