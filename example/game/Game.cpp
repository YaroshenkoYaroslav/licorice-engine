#include "Game.hpp"
#include "LicoriceEngineJsonHelper.cpp"


Game::Game
(
    const char * const  config_pass 
)
{
  Init( config_pass );
  while ( quit == 0 )
  {
    UpdateEvents();
    UpdateScene();
    Render();
    UpdateTimer();

#ifdef SHOW_FPS
    Log();
#endif
  }
}


Game::~Game
(

)
{
  int32_t  i;


  for ( i = 0; i < m_world . textures_count; ++i )
  {
    delete m_world . textures[ i ] . pixels;
  }

  for ( i = 0; i < m_world . light_sources_count; ++i )
  {
    delete m_world . light_sources[ i ];
  }

  
  delete [] m_world . shapes;
  delete [] m_world . textures;
  
  delete [] m_world . light_map;
  delete [] m_world . light_sources;
  
  delete [] m_world . map;

  
  SDL_DestroyTexture( renderer_sdl_texture );
  SDL_DestroyRenderer( renderer );
  SDL_DestroyWindow( window );
  SDL_Quit();
}
  



void
Game::Init
(
    const char * const  config_pass
)
{
  InitSDL();
  InitScene( config_pass );
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


void
Game::InitScene
(
    const char * const  config_pass
)
{
  quit = ! LoadSceneFromConfig( config_pass );
  

  m_player . o_world = & m_world;
  m_player . m_rigidbody . o_world = & m_world;
  
  m_player . moving_speed = 5;
  m_player . rotating_speed = 0.15;
  
  m_player . step_height = 0.25;
  m_player . collision_radius = 0.05;
  
  m_player . m_rigidbody . mass = 0.1;
  m_player . m_rigidbody . gravity = 9.8;

  m_player . m_camera . portals_deapth = 50;
}


bool
Game::LoadSceneFromConfig
(
    const char * const  config_pass
)
{
  nlohmann::json          m_json;
  std::ifstream           inf;


  inf.open( config_pass );

  if ( ! inf . is_open() )
  {
    std::cerr << "Can't find config: \"" << config_pass << "\"" << std::endl;
    return false;
  }
  
  inf >> m_json;
  inf . close();
  
  LoadMapFromConfig( m_json ); 
  
  LoadLightMapFromConfig( m_json );

  LoadHittablesFromConfig( m_json );
  
  LoadCameraFromConfig( m_json );
  

  return LoadTexturesFromConfig( m_json );
}
  
void
Game::LoadMapFromConfig
(
    nlohmann::json &  m_json  
)
{
  nlohmann::basic_json<>  x_json;
  nlohmann::basic_json<>  y_json;
  int32_t                 x;
  int32_t                 y;
  


  
  m_world . map_width = m_json[ "map_width" ];
  m_world . map_height = m_json[ "map_height" ];
  
  m_world . map = new LicEngine::Hittable[
    m_world . map_width * m_world . map_height
  ];

  for ( y = 0; y < m_world . map_height; ++y )
  {
    y_json = m_json[ "map" ][ std::to_string( y ) ];
    for ( x = 0; x < m_world . map_width; ++x )
    {
      x_json = y_json[ std::to_string( x ) ];

      m_world . map[ x + y * m_world . map_width ] = {
        x_json[ 0 ], x_json[ 1 ]
      };
    }
  }
}

void
Game::LoadLightMapFromConfig
(
    nlohmann::json &  m_json  
)
{
  LicEngine::LightPoint *  new_light_point;


  
  m_world . light_map = new double[
    m_world . map_width * m_world . map_height
  ];
  m_world . standart_light = 0.1;


  m_world . light_sources_count = 1;

  m_world . light_sources = new LicEngine::LightSource *[
    m_world . light_sources_count
  ];
  new_light_point = new LicEngine::LightPoint;

  new_light_point -> position_x = 11;
  new_light_point -> position_y = 20;
  new_light_point -> radius = 3;
  new_light_point -> intensity = 0.6;
  
  m_world . light_sources[ 0 ] = new_light_point;
  
  m_world . UpdateLightMap();
}

bool
Game::LoadTexturesFromConfig
(
    nlohmann::json &  m_json  
)
{
  LicEngine::Texture  new_shape_texture;
  int                 index;



  index = 0;
  m_world . textures_count = m_json[ "textures_count" ];
  m_world . textures = new LicEngine::Texture[ m_world . textures_count ];

  for ( const std::string & texture_pass : m_json[ "textures" ] ) {
    new_shape_texture = LoadTextureFromFile( texture_pass . c_str() );
    
    if ( new_shape_texture . pixels == nullptr )  return false;
    
    m_world . textures[ index++ ] = new_shape_texture;
  }

  return true;
}


void
Game::LoadHittablesFromConfig
(
    nlohmann::json &  m_json  
)
{
  int  index;
  



  index = 0;
  m_world . shapes_count = m_json[ "shapes_count" ];
  m_world . shapes = new LicEngine::Shape[ m_world . shapes_count ];
  for ( const LicEngine::Shape & c_shape : m_json[ "shapes" ] ) {
    m_world . shapes[ index++ ] = c_shape;
  }
  

  index = 0;
  m_world . portals_count = m_json[ "portals_count" ];
  m_world . portals = new LicEngine::Portal[ m_world . portals_count ];

  for ( const LicEngine::Portal & c_portal : m_json[ "portals" ] ) {
    m_world . portals[ index++ ] = c_portal;
  }
}
  
void
Game::LoadCameraFromConfig
(
    nlohmann::json &  m_json  
)
{
  LicEngine::Shape *     shape;
  LicEngine::Hittable *  hittable;
  int                    floor_position_x;
  int                    floor_position_y;




  m_player . m_camera . position_x = m_json[ "camera_x" ];
  m_player . m_camera . position_y = m_json[ "camera_y" ];
  

  floor_position_x = static_cast< int32_t >( m_player . m_camera . position_x );
  floor_position_y = static_cast< int32_t >( m_player . m_camera . position_y );

  hittable = & m_world . map[
    floor_position_y * m_world . map_width + floor_position_x
  ];
  shape = & m_world . shapes[ hittable -> index ];

  m_player . m_camera . position_z = shape -> floor_height + shape -> floor_z;
 
  m_player . m_camera . direction_x = m_json[ "camera_dir_x" ];
  m_player . m_camera . direction_y = m_json[ "camera_dir_y" ];
  
  m_player . m_camera . viewing_plane_x  = m_json[ "camera_view_plane_x" ];
  m_player . m_camera . viewing_plane_y  = m_json[ "camera_view_plane_y" ];
}
  




void
Game::UpdateScene
(

)
{
  m_player . Rotate( elapsed * mouse_move_dir );
  m_player . Move(
    elapsed * ( keys[ 0 ] + keys[ 1 ] ),
    elapsed * ( keys[ 2 ] + keys[ 3 ] )
  );
  m_player . m_rigidbody . Update( elapsed );
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
  




inline
void 
Game::UpdateTimer
(

)
{
  elapsed = static_cast< std::chrono::duration< double > >(
    std::chrono::high_resolution_clock::now() - m_clock
  ) . count();

  m_clock = std::chrono::system_clock::now();
}
  




inline
void 
Game::Log
(

)
{
  std::cout
    << "FPS: "
    << static_cast< int32_t >( 1.0 / elapsed ) 
    << std::endl;
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
