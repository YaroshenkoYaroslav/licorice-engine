#ifndef LICORICE_ENGINE_CAMERA_HPP
#define LICORICE_ENGINE_CAMERA_HPP

#include <cmath>
#include <iostream>

#include <LicoriceEngine/World.hpp>
#include <LicoriceEngine/System.hpp>
#include <LicoriceEngine/Color.hpp>

namespace LicEngine
{

class Camera
{
 public:
  
  void
  Render
  ( 
      Color::Uint32 * const  buffer,
      int16_t                buff_w,
      int16_t                buff_h, 
      const World &          world
  )
  const;

  // Counterclockwise camera rotation [ radians ]
  void
  Rotate
  (
      double angle
  );


 public:

  double  position_x;
  double  position_y;
  double  position_z;

  double  direction_x;
  double  direction_y;

  double  viewing_plane_x;
  double  viewing_plane_y;
  
  int8_t  portals_deapth;
};

}

#endif
