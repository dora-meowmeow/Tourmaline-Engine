include(FetchContent)

# Image
set(MAGNUM_WITH_STBIMAGEIMPORTER ON)

# Object
set(MAGNUM_WITH_UFBXIMPORTER ON)
set(MAGNUM_WITH_GLTFIMPORTER ON)

FetchContent_MakeAvailable(MagnumPlugins)
