#ifndef RIGIDBODY_HPP
#define RIGIDBODY_HPP


#include <LicoriceEngine/World.hpp>


class Rigidbody
{
 public:
  
  void
  Update
  (
      double  elapsed
  );
  
 public:

  double              mass;
  double              gravity;
  
  const double *      position_x;
  const double *      position_y;
  double *            position_z;

  LicEngine::World *  o_world;

 private:

  double force  =  0.0;
};


#endif
