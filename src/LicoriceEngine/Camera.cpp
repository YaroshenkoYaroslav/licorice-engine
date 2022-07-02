#include <LicoriceEngine/Camera.hpp>

namespace LicEngine
{

void
Camera::Render
(
    Uint32 * const  buffer,
    int16_t         buff_w,
    int16_t         buff_h, 
    const World &   world
)
{
  int8_t    step_x;
  int8_t    step_y;
  
  int8_t    side;
  
  int16_t   u;
  int16_t   v;
  
  int16_t    line_mask;
  int16_t    line_y1;
  int16_t    line_y2;

  int32_t   texture_u;
  int32_t   texture_v;

  Uint32    color;

  int64_t   map_x;
  int64_t   map_y;

  double    basic_wall_line_len;
  double    wall_line_len;
  double    old_wall_line_len;
  
  double    camera_x;
  
  double    ray_dir_x;
  double    ray_dir_y;
  
  double    delta_dist_x;
  double    delta_dist_y;
  
  double    side_dist_x;
  double    side_dist_y;
  
  double    hit_pos_x;
  double    hit_pos_y;

  double    local_hit_pos;
  
  double    hit_distance;

  double    texture_step;
  double    texture_pos;

  double    delta_wall_line_len;
  double    current_hit_distance;
  double    weight;
  double    current_hit_pos_x;
  double    current_hit_pos_y;


  const Hittable *  hittable;
  
  
  for ( u = 0; u < buff_w; ++u )
    for ( v = 0; v < buff_h; ++v )
      *( buffer + u + v * buff_w ) = 0;

  for ( u = 0; u < buff_w; ++u )
  {
    camera_x = 2 * u / static_cast< double >( buff_w ) - 1.0;

    ray_dir_x = direction_x + viewing_plane_x * camera_x;
    ray_dir_y = direction_y + viewing_plane_y * camera_x;

    map_x = static_cast< int64_t >( position_x );
    map_y = static_cast< int64_t >( position_y );

    delta_dist_x = std::abs( ray_dir_x ? 1 / ray_dir_x : 1e30 );
    delta_dist_y = std::abs( ray_dir_y ? 1 / ray_dir_y : 1e30 );

    
    if ( ray_dir_x < 0 )
    {
      step_x = -1;
      side_dist_x = ( position_x - map_x ) * delta_dist_x;
    }
    else
    {
      step_x = 1;
      side_dist_x = ( map_x - position_x + 1 ) * delta_dist_x;
    }
    
    if ( ray_dir_y < 0 )
    {
      step_y = -1;
      side_dist_y = ( position_y - map_y ) * delta_dist_y;
    }
    else
    {
      step_y = 1;
      side_dist_y = ( map_y - position_y + 1 ) * delta_dist_y;
    }

    
    line_mask = buff_h;

    
    while ( true )
    {
      hittable = &world . map[ map_x + map_y * world.map_width ]; 

      //Move the ray
      if ( side_dist_x < side_dist_y )
      {
        side_dist_x += delta_dist_x;
        map_x += step_x;
        side = 0;
      }
      else
      {
        side_dist_y += delta_dist_y;
        map_y += step_y;
        side = 1;
      }
      

      //Caclualte a distance
      if ( side == 0 )  hit_distance = side_dist_x - delta_dist_x;
      else              hit_distance = side_dist_y - delta_dist_y;
      hit_pos_x = position_x + hit_distance * ray_dir_x;
      hit_pos_y = position_y + hit_distance * ray_dir_y;
      
      //Calculate local hit position
      if ( side == 0 )  local_hit_pos = position_y + hit_distance * ray_dir_y;
      else              local_hit_pos = position_x + hit_distance * ray_dir_x;
      local_hit_pos -= static_cast< int64_t >( local_hit_pos );
      
      
      //Calculate a hittable's line height 
      basic_wall_line_len = hit_distance ? buff_h / hit_distance : 1e30;
      wall_line_len = basic_wall_line_len * hittable -> height;


      //Calcultate hittable's top side line points
      line_y2 = line_mask;
      line_y1 = DowngradeType< int16_t >(
        buff_h / 2 + basic_wall_line_len / 2 - wall_line_len
          + position_z * basic_wall_line_len
      );
      
      delta_wall_line_len = wall_line_len / ( line_y1 - buff_h / 2 );

      if ( line_y1 < 0 )          line_y1 = 0;
      if ( line_mask > line_y1 )  line_mask = line_y1;

      //Draw hittable's top side
      for ( v = line_y1; v < line_y2; ++v )
      {
        wall_line_len += delta_wall_line_len;
        current_hit_distance = (
          buff_h / ( 2.0 * ( v + wall_line_len ) - buff_h )
        ) * ( position_z * 2.0 + 1.0 );
        weight = current_hit_distance / hit_distance;
        current_hit_pos_x = weight * hit_pos_x + ( 1.0 - weight ) * position_x;
        current_hit_pos_y = weight * hit_pos_y + ( 1.0 - weight ) * position_y;
        current_hit_pos_x -= static_cast< int64_t >( current_hit_pos_x );
        current_hit_pos_y -= static_cast< int64_t >( current_hit_pos_y );

        texture_u = current_hit_pos_x * hittable -> m_texture -> width;
        texture_v = current_hit_pos_y * hittable -> m_texture -> height;

        color = hittable -> m_texture -> pixels[
          hittable -> m_texture -> width * texture_v + texture_u
        ];

        *( buffer + u + v * buff_w ) = color;
      }
     


      hittable = &world.map[ map_x + map_y * world.map_width ]; 

      wall_line_len = basic_wall_line_len * hittable -> height;

      //draw hittable border
      line_y2 = line_y1;
      line_y1 = DowngradeType< int16_t >(
        buff_h / 2 + basic_wall_line_len / 2 - wall_line_len + 1
          + position_z * basic_wall_line_len
      );
      
      
      if ( line_mask < line_y2 )  line_y2 = line_mask;
      if ( line_y1 < 0 )          line_y1 = 0;
      
      if ( line_mask > line_y1 )  line_mask = line_y1;


      texture_u = DowngradeType< int32_t >(
        local_hit_pos * hittable -> m_texture -> width
      );
      
      
      if ( side == 0  &&  ray_dir_x > 0 ) 
      {
        texture_u = hittable -> m_texture -> width - texture_u - 1;
      }
      
      if ( side == 1  &&  ray_dir_y < 0 )
      {
        texture_u = hittable -> m_texture -> width - texture_u - 1;
      }

      
      texture_step = hittable -> m_texture -> height / wall_line_len;

      //for basic wall_line_len 
      //    -> - buff_h / 2 + basic_wall_line_len / 2
      //for current hittable height
      //    -> (wall_line_len - buff_h - (
      //      [bottom part] basic_wall_line_len / 2 - buff_h / 2
      //    )
      texture_pos = (
        line_y1 - buff_h / 2 - basic_wall_line_len / 2 + wall_line_len
          - position_z * basic_wall_line_len
      ) * texture_step;

     
      for ( v = line_y1; v < line_y2; ++v )
      {
        texture_v = static_cast< int32_t >( texture_pos );
        texture_pos += texture_step;

        color = hittable -> m_texture -> pixels[
          hittable -> m_texture -> width * texture_v + texture_u
        ];

        if ( side == 1 )  color = ( color >> 1 ) & 0x7F7F7F7Fu;

        *( buffer + u + v * buff_w ) = color;
      }


      if ( hittable -> height >= max_see_through_height )  break;
    }
  }
}


void
Camera::Rotate
(
    double angle
)
{
  //   Use rotating matrix:
  //  | cos(angle) 	sin(angle)|
  //  |-sin(angle) 	cos(angle)|
  
  double  cos_angle           =   std::cos(angle);
  double  sin_angle           =   std::sin(angle);
  double  old_direction_x     =   direction_x;
  double  old_viewing_plane_x =   viewing_plane_x;

  direction_x     = direction_x         *  cos_angle  +  direction_y     * sin_angle;
  direction_y     = old_direction_x     * -sin_angle  +  direction_y     * cos_angle;

  viewing_plane_x = viewing_plane_x     *  cos_angle  +  viewing_plane_y * sin_angle;
  viewing_plane_y = old_viewing_plane_x * -sin_angle  +  viewing_plane_y * cos_angle;
}

}
