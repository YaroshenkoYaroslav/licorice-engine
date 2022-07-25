#ifndef MAP_EDITOR_HPP
#define MAP_EDITOR_HPP

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
#include <LicoriceEngine/LightSource.hpp>
#include <LicoriceEngine/Color.hpp>

namespace LicEngine
{
  NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
    Shape,
    floor_border, floor_top, 
    ceil_border, ceil_bottom,
    floor_height, floor_z,
    ceil_height, ceil_z
  )
  NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
    Portal,
    target_x, target_y 
  )
}

namespace MapEditor
{


struct Texture
{
  SDL_Texture *  sdl_texture;
  std::string    texture_pass;
};

struct Label
{
  std::string  title;
  float        color  [ 3 ];
  
  int32_t      hittable_type;
};

class ImLightSource
  : virtual public LicEngine::LightSource
{
 public:
  ImLightSource( std::string title = "New light source" );
  virtual void Render () = 0;
  virtual int GetType() = 0;
  virtual nlohmann::json GenerateJson() = 0;
 public:
  std::string title;
};

class ImLightPoint
  : public LicEngine::LightPoint,
    public ImLightSource
{
 public:
  ImLightPoint( std::string title = "New light point" );
  ImLightPoint( ImLightPoint &&  other );
  void Render () override;
  int GetType() override { return 0; };
  nlohmann::json GenerateJson() override;
};

class ImLightArea
  : public LicEngine::LightArea,
    public ImLightSource
{
 public:
  ImLightArea( std::string title = "New light area" );
  ImLightArea( ImLightArea &&  other );
  void Render () override;
  int GetType() override { return 1; };
  nlohmann::json GenerateJson() override;
};




NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
  ImLightPoint,
  position_x, position_y,
  radius,
  intensity,
  title
)

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
  ImLightArea,
  x1, y1,
  x2, y2,
  intensity,
  title
)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
  Label,
  title,
  color,
  hittable_type
)




//
// Init
//

void
InitSDL
(

);

void
InitImGui
(

);

void
LoadMapEditorFromConfig
(
    const char *  pass
);

void
CreateMapEditorFromInput
(

);

void
InitMapEditorScene
(

);

//
// Update
//

void
Update
(

);

void
UpdateSDLEvents
(

);

void
SelectCellByMousePosition
(

);

//
// Render
//

void
RenderMap
(

);

void
RenderImGui
(

);

void
RenderSceneSetttings
(

);

void
RenderCameraSettings
(

);

// Map settings

void
RenderMapSettings
(

);

void
MoveMap
(

);

void
ResizeMap
(

);

// Config settings

void
RenderConfigSettings
(

);

void
InitEditorMapConfig
(

);

void
InitGameMapConfig
(

);

// Textures settings

void
RenderTexturesSettings
(

);

void
RenderTexturesList
(

);


void
RenderLightSettings
(

);

void
UpdateLightMap
(

);

// Label settings

void
RenderLabelsSettings
(

);

void
RenderLabelSettings
(
    Label &  label,
    int32_t  label_id
);

void
RenderShapeSettings
(
    LicEngine::Shape & shape
);

void
RenderPortalSettings
(
    LicEngine::Portal & portal
);


void
RenderSelectedCellSettings
(

);


void
FreeMemory
(

);


SDL_Texture *
LoadSDLTextureFromFile
(
    SDL_Renderer *  renderer,
    std::string     img_pass
);
 

inline
LicEngine::Color::Uint32
F3toUINT32
(
    const float ( & c_array )[ 3 ]
);





//WINDOW CONST
constexpr const int     window_width   = 800;
constexpr const int     window_height  = 600;
constexpr const char *  window_title   = "Licorice Engine: Map editor";



//SLIDER CONST
const int     hittables_types_count = 2;
const char *  hittables_types_names  [ hittables_types_count ] = 
{ 
  "Shape", "Portal"
};

const int     light_sources_types_count = 2;
const char *  light_sources_types_names  [ light_sources_types_count ] = 
{ 
  "LightPoint", "LightArea"
};




//
// PRIMITIVE
//
void *    pixels;

bool      is_save_map_editor_data;

int8_t    quit;
int8_t    horizontal_dir  [ 2 ] {};
int8_t    vertical_dir    [ 2 ] {};

int32_t   texture_size;

int32_t   pixels_pitch;

int32_t   map_pos_x;
int32_t   map_pos_y;
int32_t   map_moving_x;
int32_t   map_moving_y;

int32_t   x;
int32_t   y;
int32_t   map_width;
int32_t   map_height;
int32_t   new_map_width;
int32_t   new_map_height;

int32_t   selected_cell_index;
int32_t   selected_cell_x;
int32_t   selected_cell_y;
int32_t   copy_cell_x;
int32_t   copy_cell_y;
  
int32_t   last_map_pos_x;
int32_t   last_map_pos_y;

int32_t   new_cell_label_id;
int32_t   new_label_id;

double    camera_dir_x;
double    camera_dir_y;
double    camera_view_plane_x;
double    camera_view_plane_y;
double    scale;

double    scene_x;
double    scene_y;

double    camera_x;
double    camera_y;

double    new_cell_light;
double    luminance;

float     luminance_mix_color  [ 3 ] {};

LicEngine::Color::Uint32    last_map_pixel;
LicEngine::Color::Uint32 *  screen_pixels;

  
//
// ONLY STL
//
std::string             otf_pass;
std::string             new_texture_pass;

std::ifstream           inf;
std::ofstream           otf;

std::vector< int32_t >  map;
std::vector< double >   light_map;


//
// SDL
//

SDL_Event       event;
  
SDL_Rect        map_drawing_rect;

SDL_Window *    window;
SDL_Renderer *  renderer;
SDL_Texture *   sdl_texture;


//
// Other
//
ImGuiIO *       io;

nlohmann::json  m_json;



std::unordered_map< int32_t, LicEngine::Shape >   shapes;
std::unordered_map< int32_t, LicEngine::Portal >  portals;
std::unordered_map< int32_t, Label >              labels;
std::list< Texture >                              textures;
std::list< ImLightSource * >                      light_sources;




};

#endif
