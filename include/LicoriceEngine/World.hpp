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
  Texture * m_texture;
  double    height;
};

struct World
{
  Hittable *  map;
  int64_t     map_width;
};

}

#endif
