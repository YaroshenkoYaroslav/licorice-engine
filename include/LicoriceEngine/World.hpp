#ifndef LICORICE_ENGINE_WORLD_HPP
#define LICORICE_ENGINE_WORLD_HPP

#include <SDL2/SDL_pixels.h>

struct Texture
{
  Uint32 *      pixels = nullptr;
  int           width;
  int           height;
};

struct Hittable
{
  Texture *     m_texture;
  double        height;
};

struct World
{
  Hittable *    map;
  int           map_width;
};

#endif
