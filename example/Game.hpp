#ifndef GAME_HPP
#define GAME_HPP


#include <fstream>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <LicoriceEngine/Camera.hpp>
#include <LicoriceEngine/World.hpp>

#include <nlohmann/json.hpp>

#include "Player.hpp"




class Game
{
 public:
  
  Game
  (
  );
  ~Game
  (
  );

 private:

  void
  Init
  (
  );

  void
  InitSDL
  (
  );
  
  bool
  LoadSceneFromConfig
  (
  );


  void
  Update
  (
  );
  
  void
  UpdateEvents
  (
  );
  
  void 
  Render
  (
  );


  LicEngine::Texture
  LoadTextureFromFile
  (
      const char *  img_pass
  );

 private:

  static constexpr const int32_t       window_width  = 800;
  static constexpr const int32_t       window_height = 600;
  static constexpr const double        player_step_height = 0.25;
  static constexpr const char * const  window_title = "Licorice Engine | Example 1";
  static constexpr const char * const  config_pass = "res/example_config.json";


 private:

  LicEngine::World   m_world;
  Player             m_player;

  SDL_Window *       window;
  SDL_Renderer *     renderer;
  SDL_Texture *      renderer_sdl_texture;

  SDL_Rect           renderer_rect;
  SDL_Event          event;
  
  void *             pixels;

  int32_t            mouse_move_dir;

  int32_t            keys  [ 4 ] {};

  int32_t            quit;
  int32_t            pixels_pitch;
};


#endif
