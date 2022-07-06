#include "Game.hpp"

#include "LicoriceEngineJsonHelper.cpp"




Game::Game
(
)
{
  Init();
  while ( quit == 0 )
  {
    UpdateEvents();
    Update();
    Render();
  }
}


Game::~Game
(
)
{
  int  i;


  for ( i = 0; i < m_world . textures_count; ++i )
  {
    delete m_world . textures[ i ] . pixels;
  }
  delete [] m_world . hittables;
  delete [] m_world . textures;
  delete [] m_world . map;

  SDL_DestroyTexture( renderer_sdl_texture );
  SDL_DestroyRenderer( renderer );
  SDL_DestroyWindow( window );
  SDL_Quit();
}
  



void
Game::Init
(
)
{
  InitSDL();
  quit = ! LoadSceneFromConfig();
  renderer_rect = { 0, 0, window_width, window_height };
}


void
Game::InitSDL
(
)
{
  SDL_Init(SDL_INIT_VIDEO);

  SDL_SetRelativeMouseMode( SDL_TRUE );

  window = SDL_CreateWindow(
    "Licorice Engine | Example Game",
    SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
    window_width, window_height,
    SDL_WINDOW_SHOWN
  );
  
  renderer = SDL_CreateRenderer(
    window, -1, SDL_RENDERER_ACCELERATED
  );
  
  renderer_sdl_texture = SDL_CreateTexture(
    renderer, 
    SDL_PIXELFORMAT_RGBA8888,
    SDL_TEXTUREACCESS_STREAMING, 
    window_width, window_height
  );
}


bool
Game::LoadSceneFromConfig
(
)
{
  nlohmann::json          m_json;
  nlohmann::basic_json<>  x_json;
  nlohmann::basic_json<>  y_json;
  std::ifstream           inf;
  LicEngine::Texture      new_hittable_texture;
  int32_t                 x;
  int32_t                 y;
  int32_t                 i;





  inf.open( config_pass );

  if ( ! inf . is_open() )
  {
    return false;
  }
  
  inf >> m_json;
  inf . close();
  
  m_world . map_width = m_json[ "map_width" ];
  m_world . map_height = m_json[ "map_height" ];
  
  m_world . map = new int32_t[
    m_world . map_width * m_world . map_height
  ];
  
  i = 0;
  m_world . textures_count = m_json[ "textures_count" ];
  m_world . textures = new LicEngine::Texture[ m_world . textures_count ];

  for ( const std::string & texture_pass : m_json[ "textures" ] ) {
    new_hittable_texture = LoadTextureFromFile( texture_pass . c_str() );
    
    if ( new_hittable_texture . pixels == nullptr )
    {
      return false;
    }
    
    m_world . textures[ i++ ] = new_hittable_texture;
  }
  
  i = 0;
  m_world . hittables_count = m_json[ "hittables_count" ];
  m_world . hittables = new LicEngine::Hittable[ m_world . hittables_count ];

  for ( const LicEngine::Hittable & c_hittable : m_json[ "hittables" ] ) {
    m_world . hittables[ i++ ] = c_hittable;
  }

  for ( y = 0; y < m_world . map_height; ++y )
  {
    y_json = m_json[ "map" ][ std::to_string( y ) ];
    for ( x = 0; x < m_world . map_width; ++x )
    {
      m_world . map[ x + y * m_world . map_width ] = y_json[ 
        std::to_string(x)
      ];
    }
  }


  m_player . o_world = & m_world;
  m_player . moving_speed = 0.1;
  m_player . rotating_speed = 0.005;
  m_player . step_height = player_step_height;
  m_player . m_camera . position_x = m_json[ "player_x" ];
  m_player . m_camera . position_y = m_json[ "player_y" ];

  i = m_world . map[ static_cast< int32_t >(
    m_player . m_camera . position_x 
      + m_player . m_camera . position_y * m_world . map_width
  ) ];

  m_player . m_camera . position_z = (
    m_world . hittables[ i ] . floor_height
      + m_world . hittables[ i ] . floor_z
  );
 
  m_player . m_camera . direction_x = -1;
  m_player . m_camera . direction_y = 0;
  
  m_player . m_camera . viewing_plane_x  = 0;
  m_player . m_camera . viewing_plane_y  = 0.66;
  

  return true;
}
  



void
Game::Update
(
)
{
  m_player . Rotate( mouse_move_dir );
  m_player . Move(
    keys[ 0 ] + keys[ 1 ],
    keys[ 2 ] + keys[ 3 ]
  );
}




void
Game::UpdateEvents
(
)
{
  mouse_move_dir = 0;

  while ( SDL_PollEvent( &event ) )
  {
    switch ( event . type )
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
      case SDL_MOUSEMOTION:
        mouse_move_dir = event . motion . xrel;
        break;
    }
  }
}




void
Game::Render
(
)
{
  SDL_LockTexture( renderer_sdl_texture, NULL, &pixels, &pixels_pitch );
  
  m_player . m_camera . Render(
    reinterpret_cast< Uint32 * >( pixels ), 
    window_width, 
    window_height, 
    m_world
  );
 
  SDL_UnlockTexture( renderer_sdl_texture );
  
  SDL_RenderCopy( renderer, renderer_sdl_texture, NULL, & renderer_rect );
  SDL_RenderPresent( renderer );
}
 



LicEngine::Texture
Game::LoadTextureFromFile
(
    const char *  img_pass
)
{

  SDL_PixelFormat *   format;
  SDL_Surface *       sdl_surface;
  LicEngine::Texture  new_texture;


  format      = SDL_AllocFormat( SDL_PIXELFORMAT_RGBA8888 );
  sdl_surface = SDL_ConvertSurface( IMG_Load( img_pass ), format, 0 );

  if ( sdl_surface == nullptr )
  {
    std::cout << "Can't load " << img_pass << std::endl;
    return { nullptr };
  }

  new_texture . width = sdl_surface -> w;
  new_texture . height = sdl_surface -> h;
  new_texture . pixels = new Uint32[ sdl_surface -> w * sdl_surface -> h ];

  memcpy(
    new_texture . pixels,
    sdl_surface -> pixels,
    sdl_surface -> h * sdl_surface -> pitch
  );

  SDL_FreeSurface( sdl_surface );

  return new_texture;
}
