find_package(SDL2_image REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/extlibs/include)

add_executable(executable_example
  src/main.cpp
  src/Game.cpp
  src/Player.cpp
  src/Rigidbody.cpp
)

target_link_libraries(executable_example licengine
  -lSDL2 -lSDL2main -lSDL2_image
)
