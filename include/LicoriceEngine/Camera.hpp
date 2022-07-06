#ifndef LICORICE_ENGINE_CAMERA_HPP
#define LICORICE_ENGINE_CAMERA_HPP

#include <LicoriceEngine/World.hpp>
#include <cmath>
#include <limits>
#include <iostream>

namespace LicEngine
{

class Camera
{
 public:
  
  void
  Render
  ( 
      Uint32 * const  buffer,
      int16_t         buff_w,
      int16_t         buff_h, 
      const World &   world
  );
  
  void
  Rotate
  (
      double angle
  );


 private:
  
  template < class U, class T >
  inline
  U
  DowngradeType
  (
      T x
  );


 public:

  double  position_x;
  double  position_y;
  double  position_z;

  double  direction_x;
  double  direction_y;

  double  viewing_plane_x;
  double  viewing_plane_y;
};


#include <LicoriceEngine/Camera.ini>


}

#endif
