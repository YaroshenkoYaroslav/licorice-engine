#ifndef GAME_HPP
#define GAME_HPP


#include <fstream>
#include <vector>

#include <chrono>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <LicoriceEngine/Camera.hpp>
#include <LicoriceEngine/World.hpp>

#include <nlohmann/json.hpp>

#include "Player.hpp"


using chrono_clock_t = std::chrono::time_point< std::chrono::system_clock >;


#define SHOW_FPS


class Game
{
 public:
  
  Game
  (
      const char * const  config_pass 
  );
  ~Game
  (
  
  );

 private:

  void
  Init
  (
      const char * const  config_pass 
  );

  void
  InitScene
  (
      const char * const  config_pass 
  );

  void
  InitSDL
  (
  
  );
  
  bool
  LoadSceneFromConfig
  (
      const char * const  config_pass 
  );
  
  void
  LoadMapFromConfig
  (
      nlohmann::json &  m_json  
  );
  
  bool
  LoadTexturesFromConfig
  (
      nlohmann::json &  m_json  
  );

  void
  LoadHittablesFromConfig
  (
      nlohmann::json &  m_json  
  );
  
  void
  LoadCameraFromConfig
  (
      nlohmann::json &  m_json  
  );
  


  void
  UpdateScene
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
 
  inline
  void 
  UpdateTimer
  (
  
  );
  
  inline
  void 
  Log
  (
  
  );


  LicEngine::Texture
  LoadTextureFromFile
  (
      const char *  img_pass
  );

 private:

  static constexpr const int           window_width  = 800;
  static constexpr const int           window_height = 600;
  static constexpr const char * const  window_titile = "Licorice Engine | Example 1";
  

 private:

  LicEngine::World   m_world;
  Player             m_player;

  SDL_Window *       window;
  SDL_Renderer *     renderer;
  SDL_Texture *      renderer_sdl_texture;

  SDL_Rect           renderer_rect;
  SDL_Event          event;

  chrono_clock_t     m_clock;

  double             elapsed;
  
  int                keys  [ 4 ] {};

  int                mouse_move_dir;
  int                quit;
  int                pixels_pitch;
  
  void *             pixels;
};


#endif
