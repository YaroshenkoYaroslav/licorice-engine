#include "map_editor.hpp"

int
main
(
    int      argc,
    char **  argv
)
{
  MapEditor::InitSDL();
  MapEditor::InitImGui();


  if ( argc == 2 )  MapEditor::LoadMapEditorFromConfig( argv[ 1 ] );
  else              MapEditor::CreateMapEditorFromInput();

  MapEditor::InitMapEditorScene();


  while ( MapEditor::quit == 0 )
  {
    MapEditor::Update();
    MapEditor::UpdateSDLEvents();


    MapEditor::RenderMap();
    SDL_RenderCopy(
      MapEditor::renderer,
      MapEditor::sdl_texture,
      NULL,
      & MapEditor::map_drawing_rect
    );
    
    MapEditor::RenderImGui();
    ImGui_ImplSDLRenderer_RenderDrawData( ImGui::GetDrawData() );
    
    SDL_RenderPresent( MapEditor::renderer );
  }

  MapEditor::FreeMemory();

  return 0;
}






void
MapEditor::InitSDL
(
  
)
{
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
}



void
MapEditor::InitImGui
(

)
{
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  io = & ImGui::GetIO(); 
  ImGui::StyleColorsDark();
  ImGui_ImplSDL2_InitForSDLRenderer( window, renderer );
  ImGui_ImplSDLRenderer_Init( renderer );
}



void
MapEditor::LoadMapEditorFromConfig
(
    const char *  pass
)
{
  otf_pass = pass;

  inf.open( pass );
  inf >> m_json;
  inf.close();
  
  map_width    = m_json[ "map_width"    ];
  map_height   = m_json[ "map_height"   ];
  camera_x     = m_json[ "camera_x"     ];
  camera_y     = m_json[ "camera_y"     ];
  camera_dir_x = m_json[ "camera_dir_x" ];
  camera_dir_y = m_json[ "camera_dir_y" ];
  camera_view_plane_x = m_json[ "camera_view_plane_x" ];
  camera_view_plane_y = m_json[ "camera_view_plane_y" ];
  
  luminance = m_json[ "luminance" ];

  for ( const std::string & texture_pass : m_json[ "textures" ] ) {
    textures . push_back( {
      LoadSDLTextureFromFile( renderer, texture_pass ),
      texture_pass
    } );
  }
 
  for ( const auto & shape_json : m_json[ "shapes" ] )
  {
    shapes[ shape_json[ 0 ] ] = shape_json[ 1 ];
  }
  for ( const auto & portal_json : m_json[ "portals" ] )
  {
    portals[ portal_json[ 0 ] ] = portal_json[ 1 ];
  }
  
  for ( const auto & label_json : m_json[ "labels" ] ) {
    labels[ label_json[ 0 ] ] = label_json[ 1 ];
  }
  
  map . resize( map_width * map_height );
  
  for ( y = 0; y < map_height; ++y )
  {
    for ( x = 0; x < map_width; ++x )
    {
      map[ x + y * map_width ] = (
        m_json[ "map" ][ std::to_string(y) ][ std::to_string(x) ]
      );
    }
  }
 
  for ( const auto & light_source_json : m_json[ "light_sources" ] )
  {
    if ( light_source_json[ "type" ] == 0 )
    {
      ImLightPoint a;
      light_sources . push_back ( new ImLightPoint ( 
        static_cast< ImLightPoint >( light_source_json )
      ) );
    }
    else
    {
      light_sources . push_back ( new ImLightArea (
        static_cast< ImLightArea >( light_source_json )
      ) );
    }
  }
}



void
MapEditor::CreateMapEditorFromInput
(

)
{
  std::cout << "Enter map width: ";   std::cin >> map_width;
  std::cout << "Enter map height: ";  std::cin >> map_height;
  
  map . resize( map_width * map_height );

  camera_dir_x = -1.0;
  camera_dir_y = 0;
  camera_view_plane_x = 0;
  camera_view_plane_y = 0.66;
  camera_x = 0;
  camera_y = 0;

  luminance = 1;

  labels[ 0 ] = {  "New label",  { 1.0, 1.0, 1.0 }  };
}



void
MapEditor::InitMapEditorScene
(

)
{
  new_map_width = map_width;
  new_map_height = map_height;


  new_label_id = 0;
  new_cell_label_id = 0;
  
  selected_cell_x = 0;
  selected_cell_y = 0;
  selected_cell_index = 0;

  scene_x = 0;
  scene_y = 0;
  
  map_moving_x = 0;
  map_moving_y = 0;
   
  texture_size = 20;
  
  scale = window_height / static_cast< double >( map_height ); 
  
  map_drawing_rect = { 0, 0, window_width, window_height };

  new_cell_light = 0;

  light_map . resize( map . size() );
  UpdateLightMap();

  quit = 0;
}






void
MapEditor::Update
(

)
{
  scene_x += ( horizontal_dir[ 0 ] + horizontal_dir[ 1 ] ) * 0.5;
  scene_y += ( vertical_dir[ 0 ] + vertical_dir[ 1 ] ) * 0.5;
}






void
MapEditor::UpdateSDLEvents
(

)
{
  while ( SDL_PollEvent( & event ) )
  {
    
    ImGui_ImplSDL2_ProcessEvent( & event );
      
    switch ( event . type )
    {
      case SDL_QUIT:
        quit = 1;
        break;

      case SDL_MOUSEWHEEL:
        if ( io -> WantCaptureMouse )  break;
        
        scale += event . wheel . y;
        break;


      case SDL_MOUSEBUTTONDOWN:
        if ( io -> WantCaptureMouse )  break;
        
        if ( event . button . button == SDL_BUTTON_LEFT )
        {
          SelectCellByMousePosition();
        }
        break;


      case SDL_KEYDOWN:
        if ( io -> WantCaptureKeyboard )  break;

        switch (event . key . keysym . sym)
        {
          case SDLK_a:  horizontal_dir[ 0 ] = -1;  break;
          case SDLK_d:  horizontal_dir[ 1 ] =  1;  break;
          case SDLK_w:  vertical_dir[ 0 ]   = -1;  break;
          case SDLK_s:  vertical_dir[ 1 ]   =  1;  break;
         

          case SDLK_c:  
            copy_cell_x = selected_cell_x;
            copy_cell_y = selected_cell_y;

          case SDLK_v:
            map[ selected_cell_index ] = map[
              copy_cell_x + copy_cell_y * map_width
            ];
        }
        break;


      case SDL_KEYUP:
        switch (event . key . keysym . sym)
        {
          case SDLK_a:  horizontal_dir[ 0 ] = 0;  break;
          case SDLK_d:  horizontal_dir[ 1 ] = 0;  break;
          case SDLK_w:  vertical_dir[ 0 ]   = 0;  break;
          case SDLK_s:  vertical_dir[ 1 ]   = 0;  break;
        }
        break;
    }
  }
}



void
MapEditor::SelectCellByMousePosition
(

)
{
  map_pos_x = std::floor( event . button . x / scale + scene_x );
  map_pos_y = std::floor( event . button . y / scale + scene_y );
 
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






void
MapEditor::RenderMap
(

)
{
  int32_t                  index;



  SDL_LockTexture( sdl_texture, NULL, & pixels, & pixels_pitch );
  
  screen_pixels = reinterpret_cast< LicEngine::Color::Uint32 * >( pixels );

  for ( y = 0; y < window_height; ++y )
  {
    map_pos_y = std::floor( y / scale + scene_y );

    for ( x = 0; x < window_width; ++x )
    {
      map_pos_x = std::floor( x / scale + scene_x );
      
      if 
      (
        map_pos_x < map_width  && map_pos_x >= 0
        &&
        map_pos_y < map_height && map_pos_y >= 0
      )
      {
        if ( last_map_pos_x != map_pos_x || last_map_pos_y != map_pos_y )
        {
          index = map_pos_x + map_pos_y * map_width;

          last_map_pixel = LicEngine::Color::Mix( 
            F3toUINT32( luminance_mix_color ),
            F3toUINT32( labels[ map[ index ] ] . color ),
            light_map[ index ]
          );
          
          last_map_pos_x = map_pos_x;
          last_map_pos_y = map_pos_y;
        }

        *( screen_pixels ) = last_map_pixel;
      }
      else
      {
        *( screen_pixels ) = 0;
      }
      ++screen_pixels;
    }
  }

  SDL_UnlockTexture( sdl_texture );
}






void
MapEditor::RenderImGui
(

)
{
  ImGui_ImplSDLRenderer_NewFrame();
  ImGui_ImplSDL2_NewFrame();
  ImGui::NewFrame();
    
  ImGui::Begin( "MENU" );
  {
    if ( ImGui::CollapsingHeader( "Scene" ) )          RenderSceneSetttings();
    if ( ImGui::CollapsingHeader( "Camera" ) )         RenderCameraSettings();
    if ( ImGui::CollapsingHeader( "Map" ) )            RenderMapSettings();
    if ( ImGui::CollapsingHeader( "Config file" ) )    RenderConfigSettings();
    if ( ImGui::CollapsingHeader( "Textures" ) )       RenderTexturesSettings();
    if ( ImGui::CollapsingHeader( "Labels" ) )         RenderLabelsSettings();
    if ( ImGui::CollapsingHeader( "Light" ) )          RenderLightSettings();
    if ( ImGui::CollapsingHeader( "Selected cell" ) )  RenderSelectedCellSettings();
  }
  ImGui::End();

  ImGui::Render();
}



void
MapEditor::RenderSceneSetttings
(

)
{
  ImGui::InputDouble( "scene_x", & scene_x );
  ImGui::InputDouble( "scene_y", & scene_y );
  ImGui::InputDouble( "scale", & scale );
  ImGui::ColorEdit3( "light color", luminance_mix_color );
}



void
MapEditor::RenderCameraSettings
(

)
{
  ImGui::InputDouble( "camera_x", & camera_x );
  ImGui::InputDouble( "camera_y", & camera_y );
  ImGui::InputDouble( "camera_dir_x", & camera_dir_x );
  ImGui::InputDouble( "camera_dir_y", & camera_dir_y );
  ImGui::InputDouble( "camera_view_plane_x", & camera_view_plane_x );
  ImGui::InputDouble( "camera_view_plane_y", & camera_view_plane_y );
}



void
MapEditor::RenderMapSettings
(

)
{
  ImGui::PushID("Map");

  
  ImGui::Text("Move map");
  ImGui::InputInt( "x", & map_moving_x );
  ImGui::InputInt( "y", & map_moving_y );

  
  if ( ImGui::Button( "Move" ) )  MoveMap();

  
  ImGui::Text("Change map size");
  ImGui::InputInt( "width", & new_map_width );
  ImGui::InputInt( "height", & new_map_height );
  
  
  if ( ImGui::Button( "Resize" ) )  ResizeMap();
  ImGui::PopID();
};



void
MapEditor::MoveMap
(

)
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

void
MapEditor::ResizeMap
(

)
{
  std::vector< int32_t >  new_map;



  for ( y = 0; y < new_map_height; ++y )
  {
    for ( x = 0; x < new_map_width; ++x )
    {
      new_map[ x + y * new_map_width ] = map[ x + y * map_width ];
    }
  }
  
  new_map.resize( new_map_width * new_map_height ); 

  map = new_map;
  
  map_width = new_map_width;
  map_height = new_map_height;
}



void
MapEditor::RenderConfigSettings
(

)
{
  ImGui::InputText( "map config pass", & otf_pass );

  ImGui::Checkbox( "save map_editor data", & is_save_map_editor_data );
  
  if ( ImGui::Button( "Save" ) )
  {
    if ( is_save_map_editor_data )  InitEditorMapConfig();
    else                            InitGameMapConfig();

    otf.open( otf_pass );
    
    if ( otf . is_open())
    {
      otf << std::setw( 4 ) << m_json << std::endl;
      otf.close();
      std::cerr <<  "LOG: Save map" << std::endl;
    }
    else 
    {
      std::cerr <<  "LOG: Can't save map" << std::endl;
    }
  }
}


void
MapEditor::InitEditorMapConfig
(

)
{
  m_json.clear(); 
 
  m_json[ "camera_x" ] = camera_x;
  m_json[ "camera_y" ] = camera_y;
  m_json[ "camera_dir_x" ] = camera_dir_x;
  m_json[ "camera_dir_y" ] = camera_dir_y;
  m_json[ "camera_view_plane_x" ] = camera_view_plane_x;
  m_json[ "camera_view_plane_y" ] = camera_view_plane_y;
  
  m_json[ "map_width" ] = map_width;
  m_json[ "map_height" ] = map_height;
  
  m_json[ "luminance" ] = luminance;

  for ( y = 0; y < map_height; ++y )
  {
    for ( x = 0; x < map_width; ++x )
    {
      m_json[ "map" ][ std::to_string( y ) ][ std::to_string( x ) ] = (
        map[ x + y * map_width ]
      );
    }
  }

  
  for ( auto it = textures . begin(); it != textures . end(); ++it )
  {
    m_json[ "textures" ] . push_back( it -> texture_pass );
  }
  

  for ( auto & [ shape_id, shape ] : shapes )
  {
    m_json[ "shapes" ] . push_back( { shape_id, shape } );
  }
  
  for ( auto & [ portal_id, portal ] : portals )
  {
    m_json[ "portals" ] . push_back( { portal_id, portal } );
  }
  
  for ( auto & [ label_id, label ] : labels )
  {
    m_json[ "labels" ] . push_back( { label_id, label } );
  
  }

  for ( auto & light_source : light_sources )
  {
    m_json[ "light_sources" ] . push_back( light_source -> GenerateJson() );
  }
}

void
MapEditor::InitGameMapConfig
(

)
{
  int64_t                                 index;
  std::string                             y_str;
  std::string                             x_str;
  std::unordered_map< int32_t, int32_t >  labels_indexes;



  y = 0;
  for ( auto & [ shape_id, shape ] : shapes )
  {
    labels_indexes [ shape_id ] = y++;
  }
  y = 0;
  for ( auto & [ portal_id, portal ] : portals )
  {
    labels_indexes [ portal_id ] = y++;
  }
  


  m_json.clear(); 
 
  m_json[ "camera_x" ] = camera_x;
  m_json[ "camera_y" ] = camera_y;
  m_json[ "camera_dir_x" ] = camera_dir_x;
  m_json[ "camera_dir_y" ] = camera_dir_y;
  m_json[ "camera_view_plane_x" ] = camera_view_plane_x;
  m_json[ "camera_view_plane_y" ] = camera_view_plane_y;
  
  m_json[ "map_width" ] = map_width;
  m_json[ "map_height" ] = map_height;
  
  for ( y = 0; y < map_height; ++y )
  {
    y_str = std::to_string( y );
    for ( x = 0; x < map_width; ++x )
    {
      x_str = std::to_string( x );
      index = map_width * y + x;
      m_json[ "map" ][ y_str ][ x_str ] = {
        labels_indexes[ map[ index ] ],
        labels[ map[ index ] ] . hittable_type
      };
      m_json[ "light_map" ][ y_str ][ x_str ] = light_map[ index ];
    }
  }

  
  m_json[ "textures_count" ] = textures . size();
  for ( auto it = textures . begin(); it != textures . end(); ++it )
  {
    m_json[ "textures" ] . push_back( it -> texture_pass );
  }
  

  m_json[ "shapes_count" ] = shapes . size();
  for ( auto & [ shape_id, shape ] : shapes )
  {
    m_json[ "shapes" ] . push_back( shape );
  }
  
  m_json[ "portals_count" ] = portals . size();
  for ( auto & [ portal_id, portal ] : portals )
  {
    m_json[ "portals" ] . push_back( portal );
  }
}



void
MapEditor::RenderTexturesSettings
(

)
{
  ImGui::InputText( "texture pass", & new_texture_pass );
  
  if ( ImGui::Button( "Add texture" ) )
  {
    SDL_Texture* new_texture = LoadSDLTextureFromFile(
      renderer, new_texture_pass
    );
    if ( new_texture != NULL)
    {
      textures . push_back( { new_texture, new_texture_pass } );
    }
  }

  if (ImGui::BeginListBox("List"))
  {
    RenderTexturesList();
    ImGui::EndListBox();
  }
  
  ImGui::InputInt( "texture size", & texture_size );
  
  if ( texture_size < 0 )  texture_size = 0;
}

void
MapEditor::RenderTexturesList
(

)
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
}



void
MapEditor::RenderLightSettings
(

)
{
  ImLightSource *  c_im_light_source;
  int32_t          new_type;
  int32_t          y;



  y = 0;

  ImGui::InputDouble( "luminance", & luminance );
  
  ImGui::NewLine();

  for ( auto it = light_sources . begin(); it != light_sources . end(); ++it )
  {
    if 
    ( 
      ImGui::TreeNode(
        reinterpret_cast< void * >( static_cast< intptr_t >( y ) ),
        "%s",
        (*it) -> title . c_str()
      )
    )
    {
      ImGui::InputText( "title", & (*it) -> title );
      
      new_type = (*it) -> GetType();
      if
      (
        ImGui::SliderInt(
          "Type", & new_type,
          0, 1,
          light_sources_types_names[ new_type ]
        )
      )
      {
        if ( new_type == 0 )  (*it) = new ImLightPoint( "New light point" );
        else                  (*it) = new ImLightArea( "New light area" );
      }
      (*it) -> Render();
      if ( ImGui::Button( "Delete" ) )
      {
        it = light_sources . erase( it );
      }
      ImGui::TreePop();
    }
    ++y;
  }
  
  ImGui::NewLine();
  
  if ( ImGui::Button( "Add light source " ) )
  {
    light_sources . push_back( new ImLightPoint( "New light point" ) );
  }
  ImGui::SameLine();
  if ( ImGui::Button( "Update" ) )
  {
    UpdateLightMap();
  }
}
    
void
MapEditor::UpdateLightMap
(

)
{
  std::fill( light_map . begin(), light_map . end(), luminance ); 
  for ( const auto & light_source : light_sources )
  {
    light_source -> Use( light_map . data(), map_width, map_height );
  }
}

void
MapEditor::ImLightPoint::Render
(

)
{
  ImGui::InputInt( "x", &  position_x );
  ImGui::InputInt( "y", &  position_y );
  ImGui::InputInt( "radius", &  radius );
  ImGui::InputDouble( "intensity", &  intensity );
}

void
MapEditor::ImLightArea::Render
(

)
{
  ImGui::InputInt( "x1", &  x1 );
  ImGui::InputInt( "y1", &  y1 );
  ImGui::InputInt( "x2", &  x2 );
  ImGui::InputInt( "y2", &  y2 );
  ImGui::InputDouble( "intensity", &  intensity );
}



void
MapEditor::RenderLabelsSettings
(

)
{
  for ( auto & [ label_id, label ] : labels )
  {
    if 
    ( 
      ImGui::TreeNode(
        reinterpret_cast< void * >( static_cast< intptr_t >( label_id ) ),
        "%d | %s",
        label_id,
        label . title . c_str()
      )
    )
    {
      RenderLabelSettings( label, label_id );
      
      ImGui::NewLine();
      
      if ( label . hittable_type == 0 )
      {
        RenderShapeSettings( shapes[ label_id ] );
      }
      else
      {
        RenderPortalSettings( portals[ label_id ] );
      }
    

      ImGui::NewLine();
      
      
      if ( ImGui::Button( "Delete" ) )
      {
        shapes . erase ( label_id );
        portals . erase ( label_id );
        labels . erase ( label_id );
        ImGui::TreePop();
        break;
      }
      
      ImGui::SameLine();
      
      if ( ImGui::Button( "Copy" ) )
      {
        labels[ new_label_id ] = labels[ label_id ];
        if ( labels[ new_label_id ] . hittable_type == 0 )
        {
          shapes[ new_label_id ] = shapes[ label_id ];
        }
        else
        {
          portals[ new_label_id ] = portals[ label_id ];
        }
      }
      
      ImGui::TreePop();
    }
  }

  ImGui::NewLine();
  ImGui::NewLine();

  if ( ImGui::Button( "Create" ) )
  {
    labels[ new_label_id ] = { "", { 1.0, 1.0, 1.0 } };
  }
  ImGui::SameLine();
  ImGui::InputInt( "New ID", & new_label_id );
}


void
MapEditor::RenderLabelSettings
(
    Label &  label,
    int32_t  label_id
)
{
  ImGui::InputText( "Title", & label . title );
  if
  (
    ImGui::SliderInt(
      "Type", & label . hittable_type,
      0, 1,
      hittables_types_names[ label . hittable_type ]
    )
  )
  {
    if ( label . hittable_type == 0 )
    {
      portals . erase ( label_id );
    }
    else
    {
      shapes . erase ( label_id );
    }
  }
  
  ImGui::ColorEdit3( "Color", label . color );
}


void
MapEditor::RenderShapeSettings
(
    LicEngine::Shape & shape
)
{
  ImGui::InputInt( "floor_border", & shape . floor_border );
  ImGui::InputInt( "floor_top", & shape . floor_top );
  ImGui::InputInt( "ceil_border", & shape . ceil_border );
  ImGui::InputInt( "ceil_bottom", & shape . ceil_bottom );
  ImGui::InputDouble( "floor_height", & shape . floor_height );
  ImGui::InputDouble( "floor_z", & shape . floor_z );
  ImGui::InputDouble( "ceil_height", & shape . ceil_height );
  ImGui::InputDouble( "ceil_z", & shape . ceil_z );
}

void
MapEditor::RenderPortalSettings
(
    LicEngine::Portal & portal
)
{
  ImGui::InputInt( "target_x", & portal . target_x );
  ImGui::InputInt( "target_y", & portal . target_y );
}



void
MapEditor::RenderSelectedCellSettings
(

)
{
  ImGui::Text( "Info: " );
  ImGui::Text( "Label ID: %d", map[ selected_cell_index ] );
  ImGui::Text( "x: %d", selected_cell_x );
  ImGui::Text( "y: %d", selected_cell_y );

  ImGui::NewLine();

  ImGui::InputInt( "New label ID", & new_cell_label_id );
  ImGui::InputDouble( "New lighting level", & new_cell_light );
  
  if ( ImGui::Button ( "Update" ) )
  {
    map[ selected_cell_index ] = new_cell_label_id;
    light_map[ selected_cell_index ] = new_cell_light;
  }
}






void
MapEditor::FreeMemory
(

)
{
  for ( const Texture & e: textures )
  {
    SDL_DestroyTexture( e . sdl_texture );
  }
 
  ImGui_ImplSDLRenderer_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
  
  SDL_DestroyTexture( sdl_texture );
  SDL_DestroyRenderer( renderer );
  SDL_DestroyWindow( window );
  
  SDL_Quit();
}






inline
SDL_Texture *
MapEditor::LoadSDLTextureFromFile
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
LicEngine::Color::Uint32
MapEditor::F3toUINT32
(
    const float ( & c_array )[ 3 ]
)
{
  return LicEngine::Color::RGBAtoUINT32(
    255 * c_array[ 0 ],
    255 * c_array[ 1 ],
    255 * c_array[ 2 ],
    255
  );
}







namespace MapEditor
{

ImLightSource::ImLightSource
(
    std::string title
)
  : title( title )
{}
  

ImLightPoint::ImLightPoint
(
    std::string title
) 
  : ImLightSource( title )
{}

ImLightPoint::ImLightPoint
(
    ImLightPoint &&  other 
)
  : ImLightSource( other . title )
{
  position_x = other . position_x;
  position_y = other . position_y;
  radius = other . radius;
  intensity = other . intensity;
}
  

ImLightArea::ImLightArea
(
    std::string title 
) 
  : ImLightSource( title ) 
{}

ImLightArea::ImLightArea
(
    ImLightArea &&  other
)
  : ImLightSource( other . title )
{
  x1 = other . x1;
  y1 = other . y1;
  x2 = other . x2;
  y2 = other . y2;
  intensity = other . intensity;
}

}






nlohmann::json
MapEditor::ImLightPoint::GenerateJson
(

)
{
  nlohmann::json j = *( this );
  j["type"] = GetType();
  return j;
}


nlohmann::json
MapEditor::ImLightArea::GenerateJson
(

)
{
  nlohmann::json j = *( this );
  j["type"] = GetType();
  return j;
}
