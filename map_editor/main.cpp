#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <memory>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl.h>
#include <imgui/imgui_impl_sdlrenderer.h>
#include <imgui/imgui_stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <nlohmann/json.hpp>

#include <LicoriceEngine/World.hpp>




struct Texture
{
  SDL_Texture *  sdl_texture;
  std::string    texture_pass;
};






namespace LicEngine
{


void 
to_json
(
    nlohmann::json &  j, 
    const Hittable &  h
)
{
  j = nlohmann::json {
    { "floor_border_index", h . floor_border_index }, 
    { "floor_top_index",    h . floor_top_index    },
    { "ceil_border_index",  h . ceil_border_index  },
    { "ceil_bottom_index",  h . ceil_bottom_index  },
    { "floor_height",       h . floor_height       },
    { "floor_z",            h . floor_z            },
    { "ceil_height",        h . ceil_height        },
    { "ceil_z",             h . ceil_z             },
  };
}



void
from_json
(
    const nlohmann::json & j,
    Hittable &             h
)
{
    j . at( "floor_border_index" ) . get_to( h . floor_border_index );
    j . at( "floor_top_index" )    . get_to( h . floor_top_index );
    j . at( "ceil_border_index" )  . get_to( h . ceil_border_index );
    j . at( "ceil_bottom_index" )  . get_to( h . ceil_bottom_index );
    j . at( "floor_height" )       . get_to( h . floor_height );
    j . at( "floor_z" )            . get_to( h . floor_z );
    j . at( "ceil_height" )        . get_to( h . ceil_height );
    j . at( "ceil_z" )             . get_to( h . ceil_z );
}


}






inline
SDL_Texture *
LoadSDLTextureFromFile
(
    SDL_Renderer *  renderer,
    std::string     img_pass
)
{
  SDL_PixelFormat  *  format;
  SDL_Surface      *  sdl_surface;
  SDL_Texture      *  sdl_texture;
  


  format      = SDL_AllocFormat( SDL_PIXELFORMAT_RGBA8888 );
  sdl_surface = SDL_ConvertSurface( IMG_Load( img_pass . c_str() ), format, 0 );


  if ( sdl_surface == nullptr )
  {
    std::cout << "Can't load " << img_pass << std::endl;
  }
  
  sdl_texture = SDL_CreateTextureFromSurface ( renderer, sdl_surface );
  
  SDL_FreeSurface( sdl_surface );

  return sdl_texture;
}





inline
Uint32
FRGBAtoURGBA
(
     const float * const  fcolor
)
{
  return    static_cast< Uint32 > ( fcolor[ 0 ] * 255 ) << 24
          | static_cast< Uint32 > ( fcolor[ 1 ] * 255 ) << 16
          | static_cast< Uint32 > ( fcolor[ 2 ] * 255 ) << 8
          | static_cast< Uint32 > ( fcolor[ 3 ] * 255 );
}





int
main
(
    int      argc,
    char **  argv
)
{
  constexpr const int  window_width  = 800;
  constexpr const int  window_height = 600;

  constexpr const char *  window_title = "Licorice Engine: Map editor";






  void *                              pixels;

  bool                                is_save_map_editor_data;

  int8_t                              quit;
  int8_t                              horizontal_dir  [ 2 ] {};
  int8_t                              vertical_dir    [ 2 ] {};

  int32_t                             texture_size;

  int32_t                             pixels_pitch;

  int32_t                             map_pos_x;
  int32_t                             map_pos_y;
  int32_t                             map_moving_x;
  int32_t                             map_moving_y;

  int32_t                             x;
  int32_t                             y;
  int32_t                             player_x;
  int32_t                             player_y;
  int32_t                             map_width;
  int32_t                             map_height;
  int32_t                             new_map_width;
  int32_t                             new_map_height;
  
  int32_t                             labels_count;
  
  int32_t                             selected_cell_index;
  int32_t                             selected_cell_x;
  int32_t                             selected_cell_y;
  int32_t                             copy_cell_x;
  int32_t                             copy_cell_y;

  double                              player_angle;
  double                              scale;

  double                              camera_x;
  double                              camera_y;

  std::vector< int32_t >              map;
  
  Uint32 *                            screen_pixels;

    
  std::string                         otf_pass;
  std::string                         new_texture_pass;

  std::ifstream                       inf;
  std::ofstream                       otf;
  

  std::list< Texture >                textures;
  std::vector< LicEngine::Hittable >  hittables;
  std::vector< float * >              labels_colors;


  SDL_Event                           event;
    
  SDL_Rect                            map_drawing_rect;

  SDL_Window *                        window;
  SDL_Renderer *                      renderer;
  SDL_Texture *                       sdl_texture;


  ImGuiIO *                           io;

  
  nlohmann::json                      m_json;




 
  SDL_Init(SDL_INIT_VIDEO);


  window = SDL_CreateWindow(
    window_title,
    SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
    window_width, window_height,
    SDL_WINDOW_SHOWN
  );
  
  renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );
  
  sdl_texture = SDL_CreateTexture(
    renderer, 
    SDL_PIXELFORMAT_RGBA8888,
    SDL_TEXTUREACCESS_STREAMING, 
    window_width, window_height
  );

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  io = & ImGui::GetIO(); 
  ImGui::StyleColorsDark();
  ImGui_ImplSDL2_InitForSDLRenderer( window, renderer );
  ImGui_ImplSDLRenderer_Init( renderer );






  if ( argc == 2 )
  {
    otf_pass = argv[ 1 ];

    inf.open( otf_pass );
    inf >> m_json;
    inf.close();
    
    map_width    = m_json[ "map_width"    ];
    map_height   = m_json[ "map_height"   ];
    player_x     = m_json[ "player_x"     ];
    player_y     = m_json[ "player_y"     ];
    player_angle = m_json[ "player_angle" ];

    for ( const std::string & texture_pass : m_json[ "textures" ] ) {
      textures . push_back( {
        LoadSDLTextureFromFile( renderer, texture_pass ),
        texture_pass
      } );
    }
    
    for ( const LicEngine::Hittable & hittable : m_json[ "hittables" ] )
    {
      hittables . push_back( hittable );
    }
    
    for ( const auto & label_color_j : m_json[ "labels_colors" ] ) {
      labels_colors . push_back( new float [] {
        label_color_j[ 0 ],
        label_color_j[ 1 ],
        label_color_j[ 2 ],
        label_color_j[ 3 ],
      } );
    }
    
    labels_count = hittables.size();
  }
  else
  {
    std::cout << "Enter map width: ";  std::cin >> map_width;
    std::cout << "Enter map height: "; std::cin >> map_height;

    player_angle = 0;
    player_x = 0;
    player_y = 0;

    hittables . push_back( {} );

    labels_colors . push_back ( new float [] { 1.0, 1.0, 1.0, 1.0 } );

    labels_count = 1;
  }


  map . resize( map_width * map_height );
  new_map_width = map_width;
  new_map_height = map_height;
  

  if ( argc == 2 )
  {
    for ( y = 0; y < map_height; ++y )
    {
      for ( x = 0; x < map_width; ++x )
      {
        map[ x + y * map_width ] = (
          m_json[ "map" ][ std::to_string(y) ][ std::to_string(x) ]
        );
      }
    }
  }
  



  selected_cell_index = 0;
  selected_cell_x = 0;
  selected_cell_y = 0;

  camera_x = 0;
  camera_y = 0;
  map_moving_x = 0;
  map_moving_y = 0;
  
  scale = window_height / static_cast< double >( map_height ); 

  texture_size = 20;
  
  map_drawing_rect = { 0, 0, window_width, window_height };

  
  quit = 0;







  while ( quit == 0 )
  {
    camera_x += ( horizontal_dir[ 0 ] + horizontal_dir[ 1 ] ) * 0.5;
    camera_y += ( vertical_dir[ 0 ] + vertical_dir[ 1 ] ) * 0.5;


    while ( SDL_PollEvent( & event ) )
    {
      
      ImGui_ImplSDL2_ProcessEvent( & event );
        
      if ( event . type ==  SDL_QUIT )
      {
          quit = 1;
          break;
      }

      
      switch ( event . type )
      {
        case SDL_MOUSEWHEEL:
          if ( io -> WantCaptureMouse )  break;
          
          scale += event . wheel . y;
          break;


        case SDL_MOUSEBUTTONDOWN:
          if ( io -> WantCaptureMouse )  break;
          
          if ( event . button . button == SDL_BUTTON_LEFT )
          {
            map_pos_x = std::floor( event . button . x / scale + camera_x );
            map_pos_y = std::floor( event . button . y / scale + camera_y );
           
            if
            ( 
              map_pos_x >= 0 && map_pos_x < map_width 
              &&
              map_pos_y >= 0 && map_pos_y < map_height
            )
            {
              selected_cell_index = map_pos_x + map_pos_y * map_width;
              selected_cell_x = map_pos_x;
              selected_cell_y = map_pos_y;
            }
          }
          break;


        case SDL_KEYDOWN:
          if ( io -> WantCaptureKeyboard )  break;

          switch (event . key . keysym . sym)
          {
            case SDLK_a:  horizontal_dir[ 0 ] = -1;  break;
            case SDLK_d:  horizontal_dir[ 1 ] =  1;  break;
            case SDLK_w:  vertical_dir[ 0 ] =   -1;  break;
            case SDLK_s:  vertical_dir[ 1 ] =    1;  break;
            
            case SDLK_c:  
              copy_cell_x = selected_cell_x;
              copy_cell_y = selected_cell_y;

            case SDLK_v:
              map[ selected_cell_index ] = map[
                copy_cell_x + copy_cell_y * map_width
              ];
              break;
          }
          break;


        case SDL_KEYUP:
          switch (event . key . keysym . sym)
          {
            case SDLK_a:  horizontal_dir[ 0 ] = 0;  break;
            case SDLK_d:  horizontal_dir[ 1 ] = 0;  break;
            case SDLK_w:  vertical_dir[ 0 ] =   0;  break;
            case SDLK_s:  vertical_dir[ 1 ] =   0;  break;
          }
          break;
      }
    }




    //Draw map
    SDL_LockTexture( sdl_texture, NULL, & pixels, & pixels_pitch );
    
    screen_pixels = reinterpret_cast< Uint32 * >( pixels );


    for ( y = 0; y < window_height; ++y )
    {
      map_pos_y = std::floor( y / scale + camera_y );

      for ( x = 0; x < window_width; ++x )
      {
        map_pos_x = std::floor( x / scale + camera_x );
        
        if 
        (
          map_pos_x < map_width  && map_pos_x >= 0
          &&
          map_pos_y < map_height && map_pos_y >= 0
        )
        {
          *screen_pixels = FRGBAtoURGBA(
            labels_colors[ map[ map_pos_x + map_pos_y * map_width ] ]
          );
        }
        else
        {
          *screen_pixels = 0;
        }
        ++screen_pixels;
      }
    }

    SDL_UnlockTexture( sdl_texture );
    
    SDL_RenderCopy( renderer, sdl_texture, NULL, & map_drawing_rect );

    




    
    //GUI
    ImGui_ImplSDLRenderer_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    
    ImGui::Begin( "MENU" );
    {
      if ( ImGui::CollapsingHeader( "Scene" ) )
      {
        ImGui::InputInt( "player_x", & player_x );
        ImGui::InputInt( "player_y", & player_y );
        ImGui::InputDouble( "player_angle", & player_angle );
        ImGui::InputDouble( "camera_x", & camera_x );
        ImGui::InputDouble( "camera_y", & camera_y );
        ImGui::InputDouble( "scale", & scale );
      }
      if ( ImGui::CollapsingHeader( "Map" ) )
      {
        ImGui::PushID("Map");

        ImGui::Text("Move map");
        ImGui::InputInt( "x", & map_moving_x );
        ImGui::InputInt( "y", & map_moving_y );

        if ( ImGui::Button( "Move" ) )
        {
          selected_cell_x += map_moving_x;
          selected_cell_y += map_moving_y;
          
          if ( selected_cell_x < 0 )
          {
            selected_cell_x = 0;
          }
          else if ( selected_cell_x >= map_width )
          {
            selected_cell_x = map_width - 1;
          }
          if ( selected_cell_y < 0 )
          {
            selected_cell_y = 0;
          }
          else if ( selected_cell_y >= map_height )
          {
            selected_cell_y = map_height - 1;
          }

          selected_cell_index = selected_cell_x + selected_cell_y * map_width;

          if ( map_moving_y > 0 )
          {
            for ( y = map_height - map_moving_y - 1; y >= 0; --y )
            {
              for ( x = 0; x < map_width; ++x )
              {
                map[ x + ( y + map_moving_y ) * map_width ] = map[
                  x + y * map_width
                ];
              }
            }
          }
          else if ( map_moving_y < 0 )
          {
            map_moving_y = -map_moving_y;
            for ( y = 0; y < map_height - map_moving_y - 1; ++y )
            {
              for ( x = 0; x < map_width; ++x )
              {
                map[ x + y * map_width ] = map[
                  x + ( y + map_moving_y ) * map_width
                ];
              }
            }
          }
          if ( map_moving_x > 0 )
          {
            for ( y = 0; y < map_height; ++y )
            {
              for ( x = map_width - map_moving_x - 1; x >= 0; --x )
              {
                map[ x + map_moving_x + y * map_width ] = map[
                  x + y * map_width
                ];
              }
            }
          }
          else if ( map_moving_x < 0 )
          {
            map_moving_x = -map_moving_x;
            for ( y = 0; y < map_height; ++y )
            {
              for ( x = 0; x < map_width - map_moving_x - 1; ++x )
              {
                map[ x + y * map_width ] = map[
                  x + map_moving_x + y * map_width
                ];
              }
            }
          }
          
          map_moving_x = 0;
          map_moving_y = 0;
        }

        
        ImGui::Text("Change map size");
        ImGui::InputInt( "width", & new_map_width );
        ImGui::InputInt( "height", & new_map_height );
        
        if ( ImGui::Button( "Resize" ) )
        {
          map_width = new_map_width;
          map_height = new_map_height;
          
          map.resize( map_width * map_height );
        }
        ImGui::PopID();
      }




      
      if ( ImGui::CollapsingHeader( "Config file" ) )
      {
        ImGui::InputText( "map config pass", & otf_pass );
      
        ImGui::Checkbox( "save map_editor data", & is_save_map_editor_data );
        
        if ( ImGui::Button( "Save" ) )
        {
          m_json.clear(); 
         
          for ( y = 0; y < map_height; ++y )
          {
            for ( x = 0; x < map_width; ++x )
            {
              m_json[ "map" ][ std::to_string( y ) ][ std::to_string(x) ] = (
                map[ x + y * map_width ]
              );
            }
          }
          
          m_json[ "map_width" ] = map_width;
          m_json[ "map_height" ] = map_height;
          m_json[ "player_x" ] = player_x;
          m_json[ "player_y" ] = player_y;
          m_json[ "player_angle" ] = player_angle;
          m_json[ "textures_count" ] = textures . size();
          m_json[ "hittables_count" ] = hittables . size();


          if (is_save_map_editor_data)
          {
            for ( const float * label_color : labels_colors )
            {
              m_json[ "labels_colors" ] . push_back( {
                label_color[0],
                label_color[1],
                label_color[2], 
                label_color[3] 
              });
            }
          }

          for ( auto it = textures . begin(); it != textures . end(); ++it )
          {
            m_json[ "textures" ] . push_back( it -> texture_pass );
          }

          for ( const LicEngine::Hittable & hittable : hittables )
          {
            m_json[ "hittables" ] . push_back( hittable );
          }

          

          otf.open( otf_pass );
          
          if ( otf . is_open())
          {
            otf << std::setw( 4 ) << m_json << std::endl;
            otf.close();
            printf( "LOG: Save map\n" );
          }
          else 
          {
            printf( "LOG: Can't save map\n" );
          }
        }
      }
      



      if ( ImGui::CollapsingHeader( "Textures" ) )
      {
        ImGui::InputText( "texture pass", & new_texture_pass );
        if ( ImGui::Button( "Add texture" ) )
        {
          SDL_Texture* new_texture = LoadSDLTextureFromFile(
            renderer, new_texture_pass
          );
          if ( new_texture != NULL)
          {
            textures.push_back( { new_texture, new_texture_pass } );
          }
        }

        if (ImGui::BeginListBox("List"))
        {
          y = 0;
          
          for ( auto it = textures . begin(); it != textures . end(); ++it )
          {
            ImGui::PushID(y);
            ImGui::Image( 
              it -> sdl_texture,
              ImVec2( texture_size, texture_size )
            );
            ImGui::SameLine();
            ImGui::InputText(
              std::to_string( y ) . c_str(),
              & it -> texture_pass
            );
            ImGui::SameLine();
            if ( ImGui::Button( "Delete" ) )
            {
              SDL_DestroyTexture( it -> sdl_texture );
              it = textures . erase( it );
            }
            ImGui::SameLine();
            if ( ImGui::Button( "Update" ) )
            {
              SDL_Texture* new_texture = LoadSDLTextureFromFile(
                renderer, it -> texture_pass
              );
              if ( new_texture != NULL )
              {
                SDL_DestroyTexture( it -> sdl_texture );
                it -> sdl_texture = new_texture;
              }
            }
            ImGui::PopID();
            ++y;
          }
          
          ImGui::EndListBox();
        }
        
        ImGui::InputInt( "texture size", & texture_size );
        
        if ( texture_size < 0 )  texture_size = 0;
      }




      if ( ImGui::CollapsingHeader( "Labels" ) )
      {
        for ( y = 0; y < labels_count; ++y )
        {
          if ( y == 0 )  ImGui::SetNextItemOpen( true, ImGuiCond_Once );

          if 
          ( 
            ImGui::TreeNode(
              reinterpret_cast< void * >(static_cast< intptr_t >(y) ),
              "Label %d", y
            )
          )
          {
            ImGui::InputInt( 
              "floor_border_index", & hittables[ y ] . floor_border_index
            );
            ImGui::InputInt( 
              "floor_top_index", & hittables[ y ] . floor_top_index
            );
            ImGui::InputInt( 
              "ceil_border_index", & hittables[ y ] . ceil_border_index
            );
            ImGui::InputInt( 
              "ceil_bottom_index", & hittables[ y ] . ceil_bottom_index
            );
            ImGui::InputDouble( 
              "floor_height", & hittables[ y ] . floor_height
            );
            ImGui::InputDouble(
              "floor_z", & hittables[ y ] . floor_z
            );
            ImGui::InputDouble(
              "ceil_height", & hittables[ y ] . ceil_height
            );
            ImGui::InputDouble(
              "ceil_z", & hittables[ y ] . ceil_z
            );
            
            ImGui::ColorEdit4(
              "label_color", labels_colors[ y ]
            );

            if ( ImGui::Button( "Create" ) )
            {
              hittables . push_back ( {} );
              labels_colors . push_back ( new float [] { 1.0, 1.0, 1.0, 1.0 } );
              ++labels_count;
            }
            
            ImGui::SameLine();

            if ( ImGui::Button( "Copy" ) )
            {
              hittables . push_back ( hittables [ y ] );
              labels_colors . push_back ( new float []
                {
                  labels_colors[ y ][ 0 ],
                  labels_colors[ y ][ 1 ],
                  labels_colors[ y ][ 2 ],
                  labels_colors[ y ][ 3 ]
                }
              );
              ++labels_count;
            }

            ImGui::SameLine();
            
            if ( ImGui::Button( "Delete" ) )
            {
              hittables . erase ( hittables . begin() + y );
              
              delete [] labels_colors[ y ];
              labels_colors . erase ( labels_colors . begin() + y );
              
              --labels_count;
            }
            
            ImGui::TreePop();
          }
        }
      }

      if ( ImGui::CollapsingHeader( "Selected cell" ) )
      {
        ImGui::Text( "x: %d  y: %d", selected_cell_x, selected_cell_y );

        ImGui::InputInt( "label index", & map[ selected_cell_index ] );

      }
    }
    ImGui::End();



 
    ImGui::Render();
    ImGui_ImplSDLRenderer_RenderDrawData( ImGui::GetDrawData() );
    SDL_RenderPresent( renderer );
  }






  for ( const Texture & e: textures )
  {
    SDL_DestroyTexture( e . sdl_texture );
  }
 
  for ( float * label_color : labels_colors ) {
    delete [] label_color;
  }


  ImGui_ImplSDLRenderer_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
  
  SDL_DestroyTexture( sdl_texture );
  SDL_DestroyRenderer( renderer );
  SDL_DestroyWindow( window );
  
  SDL_Quit();

  return 0;
}
