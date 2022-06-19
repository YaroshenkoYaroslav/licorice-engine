cmake_minimum_required(VERSION 3.0)

project(licorice-engine VERSION 0.0.1 LANGUAGES CXX)

include_directories(include)

set(INCROOT ${PROJECT_SOURCE_DIR}/include/LicoriceEngine)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/LicoriceEngine)

set(SOURCE_ENGINE
  ${SRCROOT}/Camera.cpp
)

add_library(Engine STATIC ${SOURCE_ENGINE})

add_executable(Executable examples/SimpleMap.cpp)

target_link_libraries(Executable Engine -lSDL2 -lSDL2main -lSDL2_image)
