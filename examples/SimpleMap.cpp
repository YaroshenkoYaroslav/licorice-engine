#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <LicoriceEngine/World.hpp>
#include <LicoriceEngine/Camera.hpp>
#include <chrono>
#include <iostream>


#define SHOW_FPS


constexpr const int     window_width    = 800;
constexpr const int     window_height   = 600;


constexpr const int     map_width       = 15;
constexpr const int     map_height      = 15;

constexpr const double  rotating_speed  = 2.5;
constexpr const double  moving_speed    = 3;


using chrono_clock = std::chrono::time_point<std::chrono::system_clock>;


Texture * 
CreateTexture (
    const char * img_pass
)
{
  SDL_PixelFormat   *   format;
  SDL_Surface       *   sdl_surface;
  Texture           *   new_texture;

  format      = SDL_AllocFormat( SDL_PIXELFORMAT_RGBA8888 );
  sdl_surface = SDL_ConvertSurface( IMG_Load( img_pass ), format, 0 );

  if ( sdl_surface == nullptr )
  {
    std::cout << "Can't load " << img_pass << std::endl;
  }

  new_texture = new Texture;
  new_texture -> width = sdl_surface -> w;
  new_texture -> height = sdl_surface -> h;
  new_texture -> pixels = new Uint32[ sdl_surface -> w * sdl_surface -> h ];

  memcpy(
    new_texture->pixels,
    sdl_surface->pixels,
    sdl_surface->h * sdl_surface->pitch
  );

  SDL_FreeSurface( sdl_surface );

  return new_texture;
}



int 
main (
    int, char **
)
{
  SDL_Init(SDL_INIT_VIDEO);


  
#ifdef SHOW_FPS
  int                count     =   0;
#endif
  int                quit      =   0;
  double             elapsed   =   0.0;

  
  int                keys[4]       {};
  int                pixels_pitch;

  void *             pixels;

  World              m_world;


  chrono_clock       clock;
  
  SDL_Event          event;
  
  SDL_Window *       window;
  SDL_Renderer *     sdl_renderer;
  SDL_Texture *      sdl_texture;
  LicEngine::Camera  camera;
  
  
  Hittable hr[7] =
  {
    { CreateTexture( "../examples/res/trak_panel_lrg1.jpg" ), 1 },
    { CreateTexture( "../examples/res/trak_panel_lrg4.jpg" ), 1 },
    { CreateTexture( "../examples/res/trak_panel_lrg5.jpg" ), 1 },
    { CreateTexture( "../examples/res/trak_panel_lrg8.jpg" ), 1 },
    { CreateTexture( "../examples/res/vehl1train2.jpg" ),     1 },
    { CreateTexture( "../examples/res/trak_tile_y.jpg" ),     0.25 },
    { CreateTexture( "../examples/res/trak_tile_g.jpg" ),     0 }
  };
  
  Hittable map[  map_width * map_height ] =
  {
    hr[0],hr[0],hr[0],hr[0],hr[0],hr[0],hr[0],hr[0],hr[0],hr[0],hr[0],hr[0],hr[0],hr[0],hr[0],
    hr[0],hr[5],hr[6],hr[6],hr[6],hr[6],hr[6],hr[6],hr[6],hr[6],hr[6],hr[6],hr[6],hr[6],hr[0],
    hr[0],hr[6],hr[6],hr[6],hr[6],hr[6],hr[6],hr[6],hr[6],hr[6],hr[6],hr[6],hr[6],hr[6],hr[0],
    hr[0],hr[6],hr[6],hr[6],hr[6],hr[6],hr[6],hr[6],hr[6],hr[6],hr[6],hr[6],hr[6],hr[6],hr[0],
    hr[0],hr[6],hr[6],hr[6],hr[6],hr[6],hr[1],hr[1],hr[1],hr[1],hr[1],hr[6],hr[6],hr[6],hr[0],
    hr[0],hr[6],hr[6],hr[6],hr[6],hr[6],hr[1],hr[5],hr[5],hr[5],hr[1],hr[6],hr[6],hr[6],hr[0],
    hr[0],hr[6],hr[6],hr[6],hr[6],hr[6],hr[1],hr[5],hr[5],hr[5],hr[1],hr[6],hr[6],hr[6],hr[0],
    hr[0],hr[6],hr[6],hr[6],hr[6],hr[6],hr[1],hr[5],hr[5],hr[5],hr[1],hr[6],hr[6],hr[6],hr[0],
    hr[0],hr[6],hr[6],hr[6],hr[6],hr[6],hr[1],hr[1],hr[5],hr[1],hr[1],hr[6],hr[6],hr[6],hr[0],
    hr[0],hr[6],hr[6],hr[6],hr[6],hr[6],hr[6],hr[6],hr[6],hr[6],hr[6],hr[6],hr[6],hr[6],hr[0],
    hr[0],hr[6],hr[6],hr[6],hr[6],hr[6],hr[6],hr[6],hr[6],hr[6],hr[6],hr[6],hr[6],hr[6],hr[0],
    hr[0],hr[6],hr[6],hr[6],hr[6],hr[6],hr[6],hr[6],hr[6],hr[6],hr[6],hr[6],hr[6],hr[6],hr[0],
    hr[0],hr[6],hr[6],hr[6],hr[6],hr[6],hr[6],hr[6],hr[6],hr[6],hr[6],hr[6],hr[6],hr[6],hr[0],
    hr[0],hr[6],hr[6],hr[6],hr[6],hr[6],hr[6],hr[6],hr[6],hr[6],hr[6],hr[6],hr[6],hr[6],hr[0],
    hr[0],hr[0],hr[0],hr[0],hr[0],hr[0],hr[0],hr[0],hr[0],hr[0],hr[0],hr[0],hr[0],hr[0],hr[0],
  };



  window = SDL_CreateWindow(
    "Licorice Engine | Example 1",
    SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
    window_width, window_height,
    SDL_WINDOW_SHOWN
  );
  
  sdl_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  
  sdl_texture = SDL_CreateTexture(
    sdl_renderer, 
    SDL_PIXELFORMAT_RGBA8888,
    SDL_TEXTUREACCESS_STREAMING, 
    window_width, window_height
  );
  

  m_world.map = map;
  m_world.map_width = map_width;


  camera.position_x = 3;
  camera.position_y = 3;
  
  camera.direction_x = -1;
  camera.direction_y = 0;
  
  camera.viewing_plane_x  = 0;
  camera.viewing_plane_y  = 0.66;
  
  
  clock = std::chrono::system_clock::now();

  while ( quit == 0 )
  {
    while ( SDL_PollEvent( &event ) )
    {
      switch ( event.type )
      {
        case SDL_QUIT:
          quit = 1;
          break;
        case SDL_KEYDOWN:
          switch ( event.key.keysym.sym )
          {
            case SDLK_w:  keys[ 0 ] =  1;   break;
            case SDLK_s:  keys[ 1 ] = -1;   break;
            case SDLK_a:  keys[ 2 ] = -1;   break;
            case SDLK_d:  keys[ 3 ] =  1;   break;
          }
          break;
        case SDL_KEYUP:
          switch (  event.key.keysym.sym  )
          {
            case SDLK_w:  keys[ 0 ] = 0;   break;
            case SDLK_s:  keys[ 1 ] = 0;   break;
            case SDLK_a:  keys[ 2 ] = 0;   break;
            case SDLK_d:  keys[ 3 ] = 0;   break;
          }
          break;
      }
    }
    
    
    camera.Rotate( ( keys[ 2 ] + keys[ 3 ] ) * elapsed * rotating_speed );
    
    camera.position_x += ( keys[ 0 ] + keys[ 1 ] )
      * camera.direction_x * elapsed * moving_speed;
    camera.position_y += ( keys[ 0 ] + keys[ 1 ] ) 
      * camera.direction_y * elapsed * moving_speed;
        

    SDL_LockTexture( sdl_texture, NULL, &pixels, &pixels_pitch );
#ifdef SHOW_FPS
    chrono_clock camera_clock = std::chrono::system_clock::now();
    float elapsed_camera;
#endif
    
    //for (int i = 0; i < 5; ++i)
    camera.Render(
      reinterpret_cast< Uint32 * >( pixels ), 
      window_width, 
      window_height - 100, 
      m_world
    );
   
#ifdef SHOW_FPS
    if ( count == 0 ) 
    {
      elapsed_camera = static_cast< std::chrono::duration< double > >(
        std::chrono::high_resolution_clock::now() - camera_clock
      ).count();
		  std::cout << "[Camera::Render] FPS: "
        << static_cast<int>(1 / elapsed_camera) << std::endl;
    }
#endif
    SDL_UnlockTexture( sdl_texture );
    

    SDL_Rect r { 0, 0, window_width, window_height };
    SDL_RenderCopy( sdl_renderer, sdl_texture, NULL, &r );
    
    SDL_RenderPresent( sdl_renderer );


#ifdef SHOW_FPS
    if (count == 0) {
      elapsed_camera = static_cast< std::chrono::duration< double > >(
        std::chrono::high_resolution_clock::now() - camera_clock
      ).count();
		  std::cout << "[Camera::Render + SDL_Render] FPS: "
        << static_cast<int>(1 / elapsed_camera) << std::endl << std::endl;
    }
    ++count;
    if (count >= 100) count = 0;
#endif
    
    elapsed = static_cast< std::chrono::duration< double > >(
      std::chrono::high_resolution_clock::now() - clock
    ).count();
    clock = std::chrono::high_resolution_clock::now();

  }


  for (int i = 0; i < 7; ++i)
  {
    delete [] hr[ i ].m_texture -> pixels;
    delete    hr[ i ].m_texture;
  }

  SDL_DestroyTexture( sdl_texture );
  SDL_DestroyRenderer( sdl_renderer );
  SDL_DestroyWindow( window );
  SDL_Quit();

  return 0;
}
