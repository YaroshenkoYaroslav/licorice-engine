#include "Player.hpp"





void
Player::Rotate
(
    double  angle
)
{
  m_camera . Rotate ( angle * rotating_speed );
}





void
Player::Move
(
    double  vertical_dor,
    double  hotizontal_dor
)
{
  double   new_camera_x;
  double   new_camera_y;
  double   z_difference;
  int32_t  i;



  new_camera_x = (
    m_camera . position_x + moving_speed * (
      vertical_dor * m_camera . direction_x 
      +
      hotizontal_dor * m_camera . viewing_plane_x
    )
  );
  new_camera_y = (
    m_camera . position_y + moving_speed * (
      vertical_dor * m_camera . direction_y
      +
      hotizontal_dor * m_camera . viewing_plane_y
    )
  );

  if ( new_camera_x < 0 )
  {
    new_camera_x = 0.01;
  }
  else if ( new_camera_x >= o_world -> map_width )
  {
    new_camera_x = o_world -> map_width - 0.01;
  }
  if ( new_camera_y < 0 )
  {
    new_camera_y = 0.01;
  }
  else if ( new_camera_y >= o_world -> map_height )
  {
    new_camera_y = o_world -> map_height - 0.01;
  }

  if
  ( 
    new_camera_x != m_camera . position_x
    ||
    new_camera_y != m_camera . position_y 
  )
  {
    i = (
      static_cast< int32_t >( new_camera_x )
        + static_cast< int32_t >( new_camera_y ) * o_world -> map_width
    );
   
    if ( o_world -> map[ i ] . m_type == LicEngine::Hittable::Type::Portal )
    {
      i = o_world -> map[ i ] . index;
      m_camera . position_z = o_world -> portals[ i ] . floor_z;
      m_camera . position_x = (
        o_world -> portals[ i ] . target_x + new_camera_x - static_cast< int32_t >( new_camera_x )
      );
      m_camera . position_y = (
        o_world -> portals[ i ] . target_y + new_camera_y - static_cast< int32_t >( new_camera_y )
      );
      return;
    }
    
    i = o_world -> map[ i ] . index;

    z_difference = (
      o_world -> shapes[ i ] . floor_z
        + o_world -> shapes[ i ] . floor_height
          - m_camera . position_z
    );

    //if ( std::abs( z_difference ) <= step_height )
    //{
      m_camera . position_x = new_camera_x;
      m_camera . position_y = new_camera_y;

      //if ( z_difference > 0 )
      //{
        m_camera . position_z = (
          o_world -> shapes[ i ] . floor_height 
            + o_world -> shapes[ i ] . floor_z
        );
      //}
    //}
  }
}
