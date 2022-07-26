cmake_minimum_required(VERSION 3.0)

project(licorice-engine VERSION 0.0.1 LANGUAGES CXX)

option(BUILD_SHARED_LIBS "Build shared libraries (.dll/.so) instead of static ones (.lib/.a)" ON)
option(LIC_ENGINE_BUILD_MAP_EDITOR OFF)
option(LIC_ENGINE_BUILD_EXAMPLE OFF)

find_package(SDL2 REQUIRED)

include_directories(include)

set(SRCROOT ${PROJECT_SOURCE_DIR}/src/LicoriceEngine)

add_library(licengine
  ${SRCROOT}/Camera.cpp
  ${SRCROOT}/World.cpp
  ${SRCROOT}/LightSource.cpp
)

install(
  TARGETS licengine
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
)

if (LIC_ENGINE_BUILD_MAP_EDITOR)
  add_subdirectory(tools/map_editor)
endif()
if(LIC_ENGINE_BUILD_EXAMPLE)
  add_subdirectory(example)
endif()

unset(LIC_ENGINE_BUILD_MAP_EDITOR CACHE)
unset(LIC_ENGINE_BUILD_EXAMPLE CACHE)
