cmake_minimum_required(VERSION 3.0)

project(licorice-engine VERSION 0.0.1 LANGUAGES CXX)

include_directories(include)
include_directories(extlibs/include)
include_directories(extlibs/include/imgui)

set(INCROOT ${PROJECT_SOURCE_DIR}/include/LicoriceEngine)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/LicoriceEngine)

set(EXTLIB_INCROOT ${PROJECT_SOURCE_DIR}/extlibs/include)
set(EXTLIB_SRCROOT ${PROJECT_SOURCE_DIR}/extlibs/src)

set(SOURCE_ENGINE
  ${SRCROOT}/Camera.cpp
  ${INCROOT}/Camera.hpp
)

set(SOURCE_EXTLIBS
  ${EXTLIB_INCROOT}/imgui/imconfig.h
  ${EXTLIB_INCROOT}/imgui/imgui.h
  ${EXTLIB_INCROOT}/imgui/imgui_internal.h
  ${EXTLIB_INCROOT}/imgui/imstb_rectpack.h
  ${EXTLIB_INCROOT}/imgui/imstb_textedit.h
  ${EXTLIB_INCROOT}/imgui/imstb_truetype.h
  ${EXTLIB_INCROOT}/imgui/imgui_impl_sdl.h
  ${EXTLIB_INCROOT}/imgui/imgui_impl_sdlrenderer.h
  ${EXTLIB_INCROOT}/imgui/imgui_stdlib.h
  
  ${EXTLIB_SRCROOT}/imgui.cpp
  ${EXTLIB_SRCROOT}/imgui_demo.cpp
  ${EXTLIB_SRCROOT}/imgui_draw.cpp
  ${EXTLIB_SRCROOT}/imgui_tables.cpp
  ${EXTLIB_SRCROOT}/imgui_widgets.cpp
  ${EXTLIB_SRCROOT}/imgui_impl_sdl.cpp
  ${EXTLIB_SRCROOT}/imgui_impl_sdlrenderer.cpp
  ${EXTLIB_SRCROOT}/imgui_stdlib.cpp


  ${EXTLIB_INCROOT}/nlohmann/json.hpp
)

add_library(Engine STATIC ${SOURCE_ENGINE})
add_library(Extlibs STATIC ${SOURCE_EXTLIBS})

add_executable(Executable example/main.cpp example/Game.cpp example/Player.cpp)
#add_executable(Executable map_editor/main.cpp)

target_link_libraries(Executable Engine Extlibs
  -lSDL2 -lSDL2main -lSDL2_image)
