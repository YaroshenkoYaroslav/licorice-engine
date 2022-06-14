#ifndef LICORICE_ENGINE_HITTABLE_HPP
#define LICORICE_ENGINE_HITTABLE_HPP

#include <SDL2/SDL_pixels.h>

struct Hittable {
  Uint32* pixels = nullptr;
  int pixels_width;
  int pixels_height;
};

#endif
