# SPDX-FileCopyrightText: Dora "cat" <cat@thenight.club>
# SPDX-License-Identifier: MPL-2.0
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at http://mozilla.org/MPL/2.0/.

cmake_minimum_required(VERSION 3.10)
cmake_policy(SET CMP0135 NEW)

project(Tourmaline VERSION 1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fsanitize=address")
endif()

include(GNUInstallDirs)
include(FetchContent)

# Original - https://github.com/novelrt/NovelRT/blob/c877c1e870d62df98935489e9682d93b009fb2fd/ThirdParty/CMakeLists.txt#L6
# Modified version by williamjcm
macro(external_dependency name)
  FetchContent_Declare(${name}
        ${ARGN}
        EXCLUDE_FROM_ALL
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
  URL https://github.com/mosra/corrade/archive/2b7251d8bd8833a12f0d9b8deffca7a290340d3c.zip
  URL_HASH SHA256=77ed07d373792ce05a64b87c84e7d4687965d6040df4e17b6e9922ca1cbd88c8
)

foreach(dep
  Corrade
)
  message(STATUS "Fetching ${dep}...")
  add_subdirectory(external/${dep})
endforeach()

# Building
add_library(${PROJECT_NAME} SHARED
  "source/Systems/ECS/Components.cpp"
  "source/Systems/ECS/World.cpp"
  "source/Systems/Logging.cpp"
  "source/Systems/Random.cpp"
  "source/Types/UUID.cpp"
)

# Actual linking
target_link_libraries(${PROJECT_NAME} PUBLIC
    Corrade::Main
    Corrade::Containers
    Corrade::Utility
    Corrade::PluginManager
)

# Module stuff
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})


target_include_directories(${PROJECT_NAME}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/headers>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>
    $<BUILD_INTERFACE:${corrade_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
    $<INSTALL_INTERFACE:include/${PROJECT_NAME}External>
)

FetchContent_GetProperties(Corrade SOURCE_DIR corrade_SOURCE_DIR)
FetchContent_GetProperties(Corrade BINARY_DIR corrade_BINARY_DIR)

install(
  DIRECTORY
  "${corrade_SOURCE_DIR}/src/Corrade/"
  "${corrade_BINARY_DIR}/src/Corrade/"
  DESTINATION "include/${PROJECT_NAME}External/Corrade"
  FILES_MATCHING
    PATTERN "*.h"
    PATTERN "*.hpp"
)

# A way to live forever
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND printf "Dedicated to my beloved Goma, I love you. - Dora" >> $<TARGET_FILE:${PROJECT_NAME}>
)

install(
  TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}Targets
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  INCLUDES DESTINATION include/${PROJECT_NAME}
)

install(
  TARGETS CorradeMain CorradeUtility CorradeContainers CorradePluginManager
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  INCLUDES DESTINATION include/${PROJECT_NAME}External
)

install(DIRECTORY headers/ DESTINATION include/${PROJECT_NAME})
install(DIRECTORY ${PROJECT_NAME}External/ DESTINATION
        include/${PROJECT_NAME}External)
