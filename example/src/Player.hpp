#ifndef PLAYER_HPP
#define PLAYER_HPP


#include <LicoriceEngine/Camera.hpp>
#include <LicoriceEngine/World.hpp>

#include "Rigidbody.hpp"


class Player
{
 public:

  Player();

  void
  Rotate
  (
      double  angle
  );
  
  void
  Move
  (
      double  vertical_dir,
      double  hotizontal_dir
  );


 public:

  LicEngine::Camera   m_camera;
  Rigidbody           m_rigidbody;

  double              moving_speed;
  double              rotating_speed;

  double              collision_radius;
  double              step_height;
  
  LicEngine::World *  o_world;
};


#endif
