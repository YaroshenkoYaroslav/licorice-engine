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
  enum class Type
  {
    Shape,
    Portal
  };
 

  int32_t  index;
  Type     m_type;
};


struct Shape
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

struct Portal
{
  int32_t  pair_index;
  double   floor_height;
  double   floor_z;
};


struct World
{
  Hittable *  map;
  int32_t     map_width;
  int32_t     map_height;

  Shape *     shapes;
  int32_t     shapes_count;

  Texture *   textures;
  int16_t     textures_count;
};

}

#endif
