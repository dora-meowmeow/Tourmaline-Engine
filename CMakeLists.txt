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
            "source/ECS/Component.cpp"
            "source/ECS/World.cpp"
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
