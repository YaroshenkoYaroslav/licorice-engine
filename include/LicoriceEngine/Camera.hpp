#ifndef LICORICE_ENGINE_CAMERA_HPP
#define LICORICE_ENGINE_CAMERA_HPP

#include <SDL2/SDL_stdinc.h>

namespace LicEngine
{

class Camera {
 public:
  void Render(Uint32 *const buffer, int buff_w, int buff_h,
              const int *const map, int map_w);

 public:
  double position_x;
  double position_y;

  double direction_x;
  double direction_y;

  double viewing_plane_x;
  double viewing_plane_y;
};

}

#endif
