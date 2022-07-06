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
        + hotizontal_dor * m_camera . viewing_plane_x
    )
  );
  new_camera_y = (
    m_camera . position_y + moving_speed * (
      vertical_dor * m_camera . direction_y
        + hotizontal_dor * m_camera . viewing_plane_y
    )
  );
    

  if
  ( 
    new_camera_x < o_world -> map_width   &&  new_camera_x >= 0 
    && 
    new_camera_y < o_world -> map_height  &&  new_camera_y >= 0
  )
  {
    i = o_world -> map[ static_cast< int32_t >(
      static_cast< int32_t >( new_camera_y ) * o_world -> map_width
        + static_cast< int32_t >( new_camera_x )
    ) ];
    z_difference = (
      o_world -> hittables[ i ] . floor_z
        + o_world -> hittables[ i ] . floor_height
          - m_camera . position_z
    );

    if ( std::abs(z_difference) <= step_height )
    {
      m_camera . position_x = new_camera_x;
      m_camera . position_y = new_camera_y;

      //if ( z_difference > 0 )
      //{
        m_camera . position_z = (
          o_world -> hittables[ i ] . floor_height 
            + o_world -> hittables[ i ] . floor_z
        );
      //}
    }
  }
}
