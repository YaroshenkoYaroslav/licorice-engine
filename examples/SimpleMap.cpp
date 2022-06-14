#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <LicoriceEngine/Hittable.hpp>
#include <LicoriceEngine/Camera.hpp>
#include <chrono>
#include <iostream>

constexpr const int window_width  = 800;
constexpr const int window_height = 600;

constexpr const int map_width  = 24;
constexpr const int map_height = 24;

Hittable * CreateHittable(const char* img_pass) {
  SDL_PixelFormat * format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
  SDL_Surface * sdl_surface = SDL_ConvertSurface(IMG_Load(img_pass), format, 0);
  if (sdl_surface == nullptr) {
    std::cout << "Can't load " << img_pass << std::endl;
  }
  Hittable * new_hittable = new Hittable;
  new_hittable->pixels_width = sdl_surface->w;
  new_hittable->pixels_height = sdl_surface->h;
  new_hittable->pixels = new Uint32[sdl_surface->w * sdl_surface->h];
  memcpy(new_hittable->pixels, sdl_surface->pixels, sdl_surface->h * sdl_surface->pitch);
  SDL_FreeSurface(sdl_surface);
  return new_hittable;
}

int main(int, char**) {
	Hittable * hr[5] = {
    CreateHittable("../examples/res/trak_panel_lrg1.jpg"),
    CreateHittable("../examples/res/trak_panel_lrg4.jpg"),
    CreateHittable("../examples/res/trak_panel_lrg5.jpg"),
    CreateHittable("../examples/res/trak_panel_lrg8.jpg"),
    CreateHittable("../examples/res/vehl1train2.jpg")
  };
  const Hittable * map[map_width * map_height] =
  {
    hr[0],hr[0],hr[0],hr[0],hr[0],hr[0],hr[0],hr[0],hr[0],hr[0],hr[0],hr[0],hr[0],hr[0],hr[0],hr[0],hr[0],hr[0],hr[0],hr[0],hr[0],hr[0],hr[0],hr[0],
    hr[0],NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,hr[0],
    hr[0],NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,hr[0],
    hr[0],NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,hr[0],
    hr[0],NULL ,NULL ,NULL ,NULL ,NULL ,hr[1],hr[1],hr[1],hr[1],hr[1],NULL ,NULL ,NULL ,NULL ,hr[2],NULL ,hr[2],NULL ,hr[2],NULL ,NULL ,NULL ,hr[0],
    hr[0],NULL ,NULL ,NULL ,NULL ,NULL ,hr[1],NULL ,NULL ,NULL ,hr[1],NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,hr[0],
    hr[0],NULL ,NULL ,NULL ,NULL ,NULL ,hr[1],NULL ,NULL ,NULL ,hr[1],NULL ,NULL ,NULL ,NULL ,hr[2],NULL ,NULL ,NULL ,hr[2],NULL ,NULL ,NULL ,hr[0],
    hr[0],NULL ,NULL ,NULL ,NULL ,NULL ,hr[1],NULL ,NULL ,NULL ,hr[1],NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,hr[0],
    hr[0],NULL ,NULL ,NULL ,NULL ,NULL ,hr[1],hr[1],NULL ,hr[1],hr[1],NULL ,NULL ,NULL ,NULL ,hr[2],NULL ,hr[2],NULL ,hr[2],NULL ,NULL ,NULL ,hr[0],
    hr[0],NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,hr[0],
    hr[0],NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,hr[0],
    hr[0],NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,hr[0],
    hr[0],NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,hr[0],
    hr[0],NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,hr[0],
    hr[0],NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,hr[0],
    hr[0],NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,hr[0],
    hr[0],hr[3],hr[3],hr[3],hr[3],hr[3],hr[3],hr[3],hr[3],NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,hr[0],
    hr[0],hr[3],NULL ,hr[3],NULL ,NULL ,NULL ,NULL ,hr[3],NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,hr[0],
    hr[0],hr[3],NULL ,NULL ,NULL ,NULL ,hr[4],NULL ,hr[3],NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,hr[0],
    hr[0],hr[3],NULL ,hr[3],NULL ,NULL ,NULL ,NULL ,hr[3],NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,hr[0],
    hr[0],hr[3],NULL ,hr[3],hr[3],hr[3],hr[3],hr[3],hr[3],NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,hr[0],
    hr[0],hr[3],NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,hr[0],
    hr[0],hr[3],hr[3],hr[3],hr[3],hr[3],hr[3],hr[3],hr[3],NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,NULL ,hr[0],
    hr[0],hr[0],hr[0],hr[0],hr[0],hr[0],hr[0],hr[0],hr[0],hr[0],hr[0],hr[0],hr[0],hr[0],hr[0],hr[0],hr[0],hr[0],hr[0],hr[0],hr[0],hr[0],hr[0],hr[0]
  };


  SDL_Init(SDL_INIT_VIDEO);
  
  SDL_Window * window = SDL_CreateWindow(
    "Licorice Engine | Example 1",
    SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
    window_width, window_height,
    SDL_WINDOW_SHOWN
  );
  
  SDL_Renderer * sdl_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  
  SDL_Texture * sdl_texture = SDL_CreateTexture(
    sdl_renderer, 
    SDL_PIXELFORMAT_RGBA8888,
    SDL_TEXTUREACCESS_STREAMING, 
    window_width, window_height
  );

  LicEngine::Camera camera;
  camera.position_x = 22;
  camera.position_y = 12;
  
  camera.direction_x = -1;
  camera.direction_y = 0;
  
  camera.viewing_plane_x = 0;
  camera.viewing_plane_y = 0.66;
  
  std::chrono::time_point<std::chrono::system_clock> clock = std::chrono::system_clock::now();
  double elapsed = 0;

  SDL_Event event;
  int quit = 0;
  int count = 0;
  int keys[4] {};
  while (quit == 0) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT:
          quit = 1;
          break;
        case SDL_KEYDOWN:
          switch (event.key.keysym.sym) {
            case SDLK_w: keys[0] =  1; break;
            case SDLK_s: keys[1] = -1; break;
            case SDLK_a: keys[2] = -1; break;
            case SDLK_d: keys[3] =  1; break;
          }
          break;
        case SDL_KEYUP:
          switch (event.key.keysym.sym) {
            case SDLK_w: keys[0] = 0; break;
            case SDLK_s: keys[1] = 0; break;
            case SDLK_a: keys[2] = 0; break;
            case SDLK_d: keys[3] = 0; break;
          }
          break;
      }
    }
    
    camera.Rotate((keys[2] + keys[3]) * elapsed * 2.5);
    camera.position_x += (keys[0] + keys[1]) * camera.direction_x * elapsed * 3;
    camera.position_y += (keys[0] + keys[1]) * camera.direction_y * elapsed * 3;
    
    
    void * pixels;
    int pitch;
    SDL_LockTexture(sdl_texture, NULL, &pixels, &pitch);
    std::chrono::time_point<std::chrono::system_clock> camera_clock = std::chrono::system_clock::now();
    float elapsed_camera;
    camera.Render(reinterpret_cast<Uint32*>(pixels), window_width, window_height, map, map_width);
    if (count == 0) {
      elapsed_camera = static_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now() - camera_clock).count();
		  std::cout << "[Camera::Render] FPS: " << static_cast<int>(1 / elapsed_camera) << std::endl;
    }

    SDL_UnlockTexture(sdl_texture);
    SDL_Rect r {0, 0, window_width, window_height};
    SDL_RenderCopy(sdl_renderer, sdl_texture, NULL, &r);
    
    SDL_RenderPresent(sdl_renderer);
    if (count == 0) {
      elapsed_camera = static_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now() - camera_clock).count();
		  std::cout << "[Camera::Render + SDL_Render] FPS: " << static_cast<int>(1 / elapsed_camera) << std::endl << std::endl;
    } 
    elapsed = static_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now() - clock).count();
		//std::cout << "FPS: " << static_cast<int>(1 / elapsed) << std::endl;
    clock = std::chrono::high_resolution_clock::now();
    ++count;
    if (count >= 100) count = 0;
  }

  for (int i = 0; i < 5; ++i) {
    delete [] hr[i]->pixels;
    delete    hr[i];
  }

  SDL_DestroyTexture(sdl_texture);
  SDL_DestroyRenderer(sdl_renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
