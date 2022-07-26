find_package(SDL2_image REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/extlibs/include)
include_directories(${PROJECT_SOURCE_DIR}/extlibs/include/imgui)

set(EXTLIB_SRCROOT ${PROJECT_SOURCE_DIR}/extlibs/src)

add_library(extlibs STATIC 
  ${EXTLIB_SRCROOT}/imgui.cpp
  ${EXTLIB_SRCROOT}/imgui_demo.cpp
  ${EXTLIB_SRCROOT}/imgui_draw.cpp
  ${EXTLIB_SRCROOT}/imgui_tables.cpp
  ${EXTLIB_SRCROOT}/imgui_widgets.cpp
  ${EXTLIB_SRCROOT}/imgui_impl_sdl.cpp
  ${EXTLIB_SRCROOT}/imgui_impl_sdlrenderer.cpp
  ${EXTLIB_SRCROOT}/imgui_stdlib.cpp
)

add_executable(executable_map_editor map_editor.cpp)

target_link_libraries(executable_map_editor extlibs licengine
  -lSDL2 -lSDL2main -lSDL2_image
)
