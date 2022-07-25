# Licorice Engine
The engine for making simple games based on raycasting technology. Map is defined as a 2D array.

There are two included types of hittables.
* Shape ( textured ceil and floor with custom height and z position )
* Portal ( ray moving to target position )

Also engine tools include map editor.

### Installation
-----
```shell
git clone https://github.com/YaroshenkoYaroslav/licorice-engine.git
cd licorice-engine
```

###### Compiling map editor
```shell
mkdir build && cd build && cmake -DMAP_EDITOR=ON .. && make
./Executable ../example/editor_map.json
```

###### Compiling game example
```shell
mkdir build && cd build && cmake .. && make
./Executable ../example/game_map.json
```

### Acknowledgements
-----
* Handled [Lode's Computer Graphics Tutorial](https://lodev.org/cgtutor) when developing the engine
* Use [Golgotha Textures](https://opengameart.org/content/golgotha-textures) for game example

### Used libraries
-----

| Program | Library |
| --- | -------------- |
| Engine | [SDL](https://github.com/libsdl-org/SDL) |
| Game example | [SDL](https://github.com/libsdl-org/SDL), [SDL_Image](https://github.com/libsdl-org/SDL_image), [json](https://github.com/nlohmann/json) |
| Map editor | [SDL](https://github.com/libsdl-org/SDL), [SDL_Image](https://github.com/libsdl-org/SDL_image), [json](https://github.com/nlohmann/json), [ImGui](https://github.com/ocornut/imgui) |

### Screenshots
-----

#### Map editor:

<img src="https://github.com/YaroshenkoYaroslav/licorice-engine/blob/main/map_editor_screenshot.png" width="200">

#### Game example:

<img src="https://github.com/YaroshenkoYaroslav/licorice-engine/blob/main/game_example_screenshot1.png" width="200">
<img src="https://github.com/YaroshenkoYaroslav/licorice-engine/blob/main/game_example_screenshot2.png" width="200">
