# Licorice Engine
The engine for making simple games based on raycasting technology. Map is defined as a 2D array.

There are three included types of hittables.
* Abstarct Hittable
* Shape ( textured ceil and floor with custom height and z position )
* Portal ( ray moving to target position )

And there are two included types of light sourcres.
* Abstarct LightSource
* LightPoint ( from light intensity to world luminance )
* LightArea ( fill the area by light intensity )

Also engine tools include map editor.

### Compile
-----
###### Dependencies
```
sdl2 sdl2_image
```

###### Compile licorice engine
```shell
git clone https://github.com/YaroshenkoYaroslav/licorice-engine.git
cd licorice-engine

mkdir build && cd build
cmake -DBUILD_SHARED_LIBS=ON .. && make
```

###### Compile [map editor]/[example]

```shell
git clone https://github.com/YaroshenkoYaroslav/licorice-engine.git
cd licorice-engine

mkdir build && cd build
cmake -DLIC_ENGINE_BUILD_MAP_EDITOR=ON -DLIC_ENGINE_BUILD_EXAMPLE=ON .. && make
```
###### Executable example
```shell
cp example/executable_example .
./executable_example ../example/game_map.json
```
###### Executable map editor
```shell
cp tools/map_editor/executable_map_editor .
./executable_map_editor ../example/editor_map.json
```

### Acknowledgements
-----
* Handled [Lode's Computer Graphics Tutorial](https://lodev.org/cgtutor) when developing the engine
* Use [Golgotha Textures](https://opengameart.org/content/golgotha-textures) for game example

### Used libraries
-----

| Program | Library |
| --- | -------------- |
| Engine | NONE |
| Game example | [SDL](https://github.com/libsdl-org/SDL), [SDL_Image](https://github.com/libsdl-org/SDL_image), [json](https://github.com/nlohmann/json) |
| Map editor | [SDL](https://github.com/libsdl-org/SDL), [SDL_Image](https://github.com/libsdl-org/SDL_image), [json](https://github.com/nlohmann/json), [ImGui](https://github.com/ocornut/imgui) |

### Screenshots
-----

##### Map editor:

<img src="https://github.com/YaroshenkoYaroslav/licorice-engine/blob/main/screenshots/map_editor_screenshot.png" width="200">

##### Game example:

<img src="https://github.com/YaroshenkoYaroslav/licorice-engine/blob/main/screenshots/game_example_screenshot.png" width="200">
