#ifndef LICORICE_ENGINE_CAMERA_HPP
#define LICORICE_ENGINE_CAMERA_HPP

#include <LicoriceEngine/Hittable.hpp>

namespace LicEngine
{

class Camera {
 public:
  void Render(Uint32 *const buffer, int buff_w, int buff_h,
              const Hittable *const *const map, int map_w);
  
  void Rotate(double angle);

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
