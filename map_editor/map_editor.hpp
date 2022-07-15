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





namespace LicEngine
{


void 
to_json
(
    nlohmann::json &  j, 
    const Shape &     s
);



void
from_json
(
    const nlohmann::json & j,
    Shape &                s
);


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
  
  int32_t      hittable_id;
  int32_t      hittable_type;
};






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



void
RenderConfigSettings
(

);

bool
InitAndSaveMapConfig
(

);



void
RenderTexturesSettings
(

);

void
RenderTexturesList
(

);



void
RenderLabelsSettings
(

);

void
RenderLabelSettings
(
    Label & label
);

void
RenderShapeSettings
(
    LicEngine::Shape & shape
);



void
RenderSelectedCellSettings
(

);




void
FreeMemory
(

);





inline
SDL_Texture *
LoadSDLTextureFromFile
(
    SDL_Renderer *  renderer,
    std::string     img_pass
);


inline
Uint32
FRGBAtoURGBA
(
     const float * const  fcolor
);





void 
to_json
(
    nlohmann::json &  j, 
    const Label &     label
);


void
from_json
(
    const nlohmann::json & j,
    Label &                label
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

double    player_angle;
double    scale;

double    player_x;
double    player_y;

double    camera_x;
double    camera_y;


Uint32    last_map_pixel;
Uint32 *  screen_pixels;

  
//
// ONLY STL
//
std::string             otf_pass;
std::string             new_texture_pass;

std::ifstream           inf;
std::ofstream           otf;

std::vector< int32_t >  map;


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



std::unordered_map< int32_t, LicEngine::Shape >  shapes;
std::unordered_map< int32_t, Label >             labels;
std::list< Texture >                             textures;




};
