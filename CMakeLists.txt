# SPDX-FileCopyrightText: Dora "cat" <cat@thenight.club>
# SPDX-License-Identifier: MPL-2.0
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at http://mozilla.org/MPL/2.0/.

cmake_minimum_required(VERSION 3.10)
cmake_policy(SET CMP0135 NEW)

project(Tourmaline VERSION 1)

include(GNUInstallDirs)
include(FetchContent)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_INSTALL_LIBDIR "${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}")
set(CMAKE_INSTALL_INCLUDEDIR "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}External")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fsanitize=address -coverage")
  set(CORRADE_CPU_USE_IFUNC "OFF")
  message(WARNING "Making a debug build do NOT ship!")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
endif()

# Original - https://github.com/novelrt/NovelRT/blob/c877c1e870d62df98935489e9682d93b009fb2fd/ThirdParty/CMakeLists.txt#L6
# Modified version by williamjcm
macro(external_dependency name)
  FetchContent_Declare(${name}
        ${ARGN}
        PREFIX "${CMAKE_CURRENT_BINARY_DIR}/${name}"
        TMP_DIR "${CMAKE_CURRENT_BINARY_DIR}/${name}/tmp"
        STAMP_DIR "${CMAKE_CURRENT_BINARY_DIR}/${name}/stamp"
        DOWNLOAD_DIR "${CMAKE_CURRENT_BINARY_DIR}/${name}/dl"
        SOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}/${name}/src"
        SUBBUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/${name}/build"
        BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/${name}/bin"
        INSTALL_DIR "${CMAKE_CURRENT_BINARY_DIR}/${name}/inst"
        LOG_DIR "${CMAKE_CURRENT_BINARY_DIR}/${name}/log"
    )
  list(APPEND CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}/cmake/${name}")
endmacro()

# Third Party Libraries
external_dependency(Corrade
  URL https://github.com/mosra/corrade/archive/783e4e4807536ec52c352986fc9317db986ace96.zip
  URL_HASH SHA256=17f3b4a0e523ad34504e99df35c018c1498afd50c72f6ce7087fda9bcb408791
)

external_dependency(Magnum
  URL https://github.com/mosra/magnum/archive/145fc82c756ba69afa1f983a914e6b4ad6d76bb3.zip
  URL_HASH SHA256=a9709905cc4c0841edd8ccbfe86e539929cfa6e010bc377179121762f3343556
)

foreach(dep
  Corrade
  Magnum
)
  message(STATUS "Fetching ${dep}...")
  add_subdirectory(external/${dep})
endforeach()

# Building
add_library(${PROJECT_NAME} SHARED
  "source/Systems/ECS/Components.cpp"
  "source/Systems/ECS/Entities.cpp"
  "source/Systems/ECS/World.cpp"
  "source/Systems/Logging.cpp"
  "source/Systems/Random.cpp"
  "source/Types/UUID.cpp"
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  target_compile_options(${PROJECT_NAME} PRIVATE "-ftime-report")
endif()

# Actual linking
target_link_libraries(${PROJECT_NAME} PUBLIC
    Corrade::Main
    Corrade::Containers
    Corrade::Utility
    Corrade::PluginManager

    Magnum::Magnum
    Magnum::Sdl2Application
    Magnum::GL
    Magnum::Trade
)

# Module stuff
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})

# Fetching source and compiled directories
FetchContent_GetProperties(Corrade SOURCE_DIR corrade_SOURCE_DIR)
FetchContent_GetProperties(Magnum SOURCE_DIR magnum_SOURCE_DIR)

# For Development
target_include_directories(${PROJECT_NAME}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/headers>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>
    $<BUILD_INTERFACE:${corrade_SOURCE_DIR}/src>
    $<BUILD_INTERFACE:${magnum_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
    $<INSTALL_INTERFACE:include/${PROJECT_NAME}External>
)

# A way to live forever
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND printf "Dedicated to my beloved Goma, I love you. - Dora" >> $<TARGET_FILE:${PROJECT_NAME}>
)

# Tourmaline Install
install(
  TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}Targets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  INCLUDES DESTINATION include/${PROJECT_NAME}
)

install(DIRECTORY headers/ DESTINATION include/${PROJECT_NAME})
install(DIRECTORY ${PROJECT_NAME}External/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
