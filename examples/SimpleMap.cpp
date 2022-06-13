#include <SDL2/SDL.h>
#include <LicoriceEngine/Camera.hpp>

constexpr const int window_width  = 800;
constexpr const int window_height = 600;

constexpr const int map_width  = 24;
constexpr const int map_height = 24;

const int map[map_width * map_height] =
{
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1,
  1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1,
  1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1,
  1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
};

int main(int, char**) {
  SDL_Init(SDL_INIT_VIDEO);

  SDL_Window * window = SDL_CreateWindow(
    "Licorice Engine | Example 1",
    SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
    window_width, window_height, 0
  );
  SDL_Surface* surface = SDL_GetWindowSurface(window);

  LicEngine::Camera camera;
  camera.position_x = 22;
  camera.position_y = 12;
  
  camera.direction_x = -1;
  camera.direction_y = 0;
  
  camera.viewing_plane_x = 0;
  camera.viewing_plane_y = 0.66;
  

  Uint32 * buffer = new Uint32[window_width * window_height];
  memset(buffer, 0, window_width * window_height * sizeof(Uint32));

  SDL_Event event;
  bool is_quit = false;
  
  while (!is_quit) {
    SDL_WaitEvent(&event);

    switch (event.type) {
        case SDL_QUIT:
            is_quit = true;
            break;
    }
    
    camera.Render(buffer, window_width, window_height, map, map_width);
    
    Uint32* surface_buffer = reinterpret_cast<Uint32*>(surface->pixels);
    for (int y = 0; y < window_height; ++y)
      for (int x = 0; x < window_width; ++x)
        *surface_buffer++ = buffer[y * window_width + x];
    SDL_UpdateWindowSurface(window);
  }

  SDL_Quit();

  delete[] buffer;
  SDL_DestroyWindow(window);

  return 0;
}
