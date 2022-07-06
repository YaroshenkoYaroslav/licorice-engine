#ifndef LICORICE_ENGINE_WORLD_HPP
#define LICORICE_ENGINE_WORLD_HPP

#include <SDL2/SDL_pixels.h>

namespace LicEngine
{

struct Texture
{
  Uint32 *  pixels = nullptr;
  int       width;
  int       height;
};

struct Hittable
{
  Texture * floor_border_t;
  Texture * floor_top_t;
  Texture * ceil_border_t;
  Texture * ceil_bottom_t;
  double    floor_height;
  double    ceil_height;
  double    ceil_z;
};

struct World
{
  Hittable *  map;
  int64_t     map_width;
  int64_t     map_height;
};

}

#endif
