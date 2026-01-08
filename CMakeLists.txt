# SPDX-FileCopyrightText: Dora "cat" <cat@thenight.club>
# SPDX-License-Identifier: MPL-2.0
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at http://mozilla.org/MPL/2.0/.

cmake_minimum_required(VERSION 3.10)

project(Tourmaline VERSION 1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fsanitize=address")
endif()

include(GNUInstallDirs)

include_directories(headers)
add_library(${PROJECT_NAME} SHARED
            "source/Systems/ECS/Component.cpp"
            "source/Systems/ECS/World.cpp"
            "source/Systems/Logging.cpp"
            "source/Systems/Random.cpp"
            "source/Types/UUID.cpp")

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})

# Nothing to link right now
target_link_libraries(${PROJECT_NAME})

install(
  TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}Targets
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin)
install(DIRECTORY headers/ DESTINATION include/${PROJECT_NAME})
