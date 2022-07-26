#include "Rigidbody.hpp"


void
Rigidbody::Update
(
    double  elapsed
)
{
  LicEngine::Shape *  shape;
  int16_t             shape_index;
  int32_t             floor_position_x;
  int32_t             floor_position_y;



  floor_position_x = static_cast< int32_t >( *( position_x ) );
  floor_position_y = static_cast< int32_t >( *( position_y ) );
  
  shape_index = o_world -> map[
    floor_position_y * o_world -> map_width + floor_position_x
  ] . index;
  shape = & o_world -> shapes[ shape_index ];
 
  *( position_z ) -= force;

  if ( *( position_z ) < shape -> floor_z + shape -> floor_height )
  {
    *( position_z ) = shape -> floor_z + shape -> floor_height;
    force = 0;
  }
  else
  {
    force += elapsed * mass * gravity;
  }
}
