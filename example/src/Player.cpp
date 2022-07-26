#include "Player.hpp"
#include "Math.cpp"
  

Player::Player
(

)
{
  m_rigidbody . position_x = & m_camera . position_x;
  m_rigidbody . position_y = & m_camera . position_y;
  m_rigidbody . position_z = & m_camera . position_z;
}





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
    double  vertical_dir,
    double  hotizontal_dir
)
{
  LicEngine::Hittable *  hittable;
  LicEngine::Shape *     shape;
  LicEngine::Portal *    portal;

  int32_t                floor_new_camera_x;
  int32_t                floor_new_camera_y;
  int32_t                floor_collision_x;
  int32_t                floor_collision_y;
  
  double                 new_camera_x;
  double                 new_camera_y;

  double                 camera_dir_x;
  double                 camera_dir_y;
 
  double                 collision_z_difference;
  double                 camera_z_difference;


  


  camera_dir_x = (
    vertical_dir * m_camera . direction_x 
      + hotizontal_dir * m_camera . viewing_plane_x
  );
  camera_dir_y = (
    vertical_dir * m_camera . direction_y
      + hotizontal_dir * m_camera . viewing_plane_y
  );
  
  new_camera_x = m_camera . position_x + moving_speed * camera_dir_x;
  new_camera_y = m_camera . position_y + moving_speed * camera_dir_y;

  
  
  if ( new_camera_x < 0 )
  {
    new_camera_x = collision_radius;
  }
  else if ( new_camera_x >= o_world -> map_width )
  {
    new_camera_x = o_world -> map_width - collision_radius;
  }
  if ( new_camera_y < 0 )
  {
    new_camera_y = collision_radius;
  }
  else if ( new_camera_y >= o_world -> map_height )
  {
    new_camera_y = o_world -> map_height - collision_radius;
  }


  
  if
  ( 
    new_camera_x != m_camera . position_x
    ||
    new_camera_y != m_camera . position_y 
  )
  {
    floor_new_camera_x = static_cast< int32_t >( new_camera_x );
    floor_new_camera_y = static_cast< int32_t >( new_camera_y );
    
    hittable = & o_world -> map[
      floor_new_camera_y * o_world -> map_width + floor_new_camera_x
    ];
   
    
    if ( hittable -> m_type == LicEngine::Hittable::Type::Portal )
    {
      portal = & o_world -> portals[ hittable -> index ];
      
      m_camera . position_x = (
        portal -> target_x + new_camera_x - floor_new_camera_x
      );
      m_camera . position_y = (
        portal -> target_y + new_camera_y - floor_new_camera_y
      );
      
      floor_new_camera_x = static_cast< int32_t >( m_camera . position_x );
      floor_new_camera_y = static_cast< int32_t >( m_camera . position_y );

      hittable = & o_world -> map[
        floor_new_camera_y * o_world -> map_width + floor_new_camera_x
      ];
      shape = & o_world -> shapes[ hittable -> index ];
     
      m_camera . position_z = shape -> floor_z + shape -> floor_height;
      
      return;
    }
  
    
    shape = & o_world -> shapes[ hittable -> index ];
    
    camera_z_difference = (
      shape -> floor_z + shape -> floor_height - m_camera . position_z
    );
   


    floor_collision_x = static_cast< int32_t >(
      new_camera_x + Math::sign( camera_dir_x ) * collision_radius
    );
    floor_collision_y = static_cast< int32_t >(
      new_camera_y + Math::sign( camera_dir_y ) * collision_radius
    );
    
    hittable = & o_world -> map[ 
      o_world -> map_width * floor_collision_y + floor_collision_x
    ];
    if ( hittable -> m_type == LicEngine::Hittable::Type::Portal )
    {
      collision_z_difference = 0;
    }
    else
    {
      shape = & o_world -> shapes[ hittable -> index ];
      collision_z_difference = (
        shape -> floor_z + shape -> floor_height - m_camera . position_z
      );
    }

   
   
    if
    ( 
      camera_z_difference <= step_height
      &&
      collision_z_difference <= step_height
    )
    {
      m_camera . position_x = new_camera_x;
      m_camera . position_y = new_camera_y;

      if ( camera_z_difference > 0 )
      {
        m_camera . position_z = shape -> floor_height + shape -> floor_z;
      }
    }
  }
}

