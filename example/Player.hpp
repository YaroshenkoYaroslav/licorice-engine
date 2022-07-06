#ifndef PLAYER_HPP
#define PLAYER_HPP


#include <LicoriceEngine/Camera.hpp>
#include <LicoriceEngine/World.hpp>




class Player
{
 public:

  void
  Rotate
  (
      double  angle
  );
  
  void
  Move
  (
      double  vertical_dor,
      double  hotizontal_dor
  );

 public:

  LicEngine::Camera   m_camera;
  LicEngine::World *  o_world;

  double              moving_speed;
  double              rotating_speed;
  double              step_height;
};


#endif
