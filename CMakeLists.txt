cmake_minimum_required(VERSION 3.0)

project(licorice-engine VERSION 0.0.1 LANGUAGES CXX)

option(MAP_EDITOR "Is map_editor" OFF)

include_directories(include)
include_directories(extlibs/include)
include_directories(extlibs/include/imgui)

set(INCROOT ${PROJECT_SOURCE_DIR}/include/LicoriceEngine)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/LicoriceEngine)

set(EXTLIB_INCROOT ${PROJECT_SOURCE_DIR}/extlibs/include)
set(EXTLIB_SRCROOT ${PROJECT_SOURCE_DIR}/extlibs/src)


if (MAP_EDITOR)
  
  add_library(Extlibs STATIC 
    ${EXTLIB_SRCROOT}/imgui.cpp
    ${EXTLIB_SRCROOT}/imgui_demo.cpp
    ${EXTLIB_SRCROOT}/imgui_draw.cpp
    ${EXTLIB_SRCROOT}/imgui_tables.cpp
    ${EXTLIB_SRCROOT}/imgui_widgets.cpp
    ${EXTLIB_SRCROOT}/imgui_impl_sdl.cpp
    ${EXTLIB_SRCROOT}/imgui_impl_sdlrenderer.cpp
    ${EXTLIB_SRCROOT}/imgui_stdlib.cpp
  )
  add_library(Engine STATIC
    ${SRCROOT}/LightSource.cpp
  )
  add_executable(Executable tools/map_editor/map_editor.cpp)
  target_link_libraries(Executable Extlibs Engine
    -lSDL2 -lSDL2main -lSDL2_image
  )

else()
  add_library(Engine STATIC
    ${SRCROOT}/Camera.cpp
    ${SRCROOT}/World.cpp
    ${SRCROOT}/LightSource.cpp
  )

  
  add_executable(Executable
    example/game/main.cpp
    example/game/Game.cpp
    example/game/Player.cpp
    example/game/Rigidbody.cpp
  )
  
  target_link_libraries(Executable Engine
    -lSDL2 -lSDL2main -lSDL2_image -O2
  )

endif()

unset(MAP_EDITOR CACHE)
