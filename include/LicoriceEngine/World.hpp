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
  int32_t   floor_border_index;
  int32_t   floor_top_index;
  int32_t   ceil_border_index;
  int32_t   ceil_bottom_index;
  double    floor_height;
  double    floor_z;
  double    ceil_height;
  double    ceil_z;
};

struct World
{
  int32_t *   map;
  int32_t     map_width;
  int32_t     map_height;

  Hittable *  hittables;
  int32_t     hittables_count;

  Texture *   textures;
  int16_t     textures_count;
};

}

#endif
