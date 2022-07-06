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
  
  int16_t   bottom_line_mask;
  int16_t   top_line_mask;
  int16_t   unmusk_line_y;
  int16_t   line_y1;
  int16_t   line_y2;

  int32_t   texture_u;
  int32_t   texture_v;

  Uint32    color;

  int64_t   map_x;
  int64_t   map_y;

  double    basic_wall_line_len;
  double    wall_line_len;
  
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

  double    act_distance_correction;
  double    current_hit_distance;
  double    weight;
  double    current_hit_pos_x;
  double    current_hit_pos_y;

  const Hittable *  hittable;
  const Hittable *  old_hittable;
  

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

    
    top_line_mask = buff_h;
    bottom_line_mask = 0;
   

    while ( true )
    {
      old_hittable = &world . map[ map_x + map_y * world.map_width ];

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
      
      hittable = &world . map[ map_x + map_y * world.map_width ]; 
      

      //Caclualte a distance
      if ( side == 0 )  hit_distance = side_dist_x - delta_dist_x;
      else              hit_distance = side_dist_y - delta_dist_y;
      hit_pos_x = position_x + hit_distance * ray_dir_x;
      hit_pos_y = position_y + hit_distance * ray_dir_y;
      
      //Calculate local hit position
      if ( side == 0 )  local_hit_pos = hit_pos_y;
      else              local_hit_pos = hit_pos_x;
      local_hit_pos -= static_cast< int64_t >( local_hit_pos );
      
      basic_wall_line_len = hit_distance ? buff_h / hit_distance : 1e30;
      

      
      //
      //  CEIL BOTTOM FACE
      //
      wall_line_len = basic_wall_line_len * old_hittable -> ceil_height;
      
      line_y1 = bottom_line_mask;
      line_y2 = DowngradeType< int16_t >( std::round (
        buff_h / 2.0 + wall_line_len
          - basic_wall_line_len * ( old_hittable -> ceil_z - position_z - 0.5 )
      ) );

      if ( line_y2 > top_line_mask )
      {
        line_y2 = top_line_mask;
      }
      if ( bottom_line_mask < line_y2 )
      {
        bottom_line_mask = line_y2;
      }

      double act_distance_correction = (
        2.0 * old_hittable -> ceil_z - 2 * position_z  - 3.0
      );
      for ( v = line_y1; v < line_y2; ++v )
      {
        current_hit_distance = buff_h / ( buff_h - 2.0 * v );
        current_hit_distance *= act_distance_correction;
        weight = current_hit_distance / hit_distance;
        current_hit_pos_x = weight * hit_pos_x + ( 1.0 - weight ) * position_x;
        current_hit_pos_y = weight * hit_pos_y + ( 1.0 - weight ) * position_y;
        current_hit_pos_x -= static_cast< int64_t >( current_hit_pos_x );
        current_hit_pos_y -= static_cast< int64_t >( current_hit_pos_y );

        texture_u = current_hit_pos_x * old_hittable -> ceil_bottom_t -> width;
        texture_v = current_hit_pos_y * old_hittable -> ceil_bottom_t -> height;

        color = old_hittable -> ceil_bottom_t -> pixels[
          old_hittable -> ceil_bottom_t -> width * texture_v + texture_u
        ];

        *( buffer + u + v * buff_w ) = color;
      }



      //
      //  CEIL WALL
      //
      wall_line_len = basic_wall_line_len * hittable -> ceil_height;

      unmusk_line_y = line_y2;
      line_y1 = unmusk_line_y;
      line_y2 = DowngradeType< int16_t >( std::round (
        buff_h / 2.0 + wall_line_len
          - basic_wall_line_len * ( hittable -> ceil_z - position_z - 0.5 )
      ) );
      
      
      if ( bottom_line_mask > line_y1 )
      {
        line_y1 = bottom_line_mask;
      }
      if ( line_y2 > top_line_mask )
      {
        line_y2 = top_line_mask;
      }
      
      if ( bottom_line_mask < line_y2 )
      {
        bottom_line_mask = line_y2;
      }


      texture_u = DowngradeType< int32_t >(
        local_hit_pos * hittable -> ceil_border_t -> width
      );
      
      
      if ( side == 0  &&  ray_dir_x > 0 ) 
      {
        texture_u = hittable -> ceil_border_t -> width - texture_u - 1;
      }
      
      if ( side == 1  &&  ray_dir_y < 0 )
      {
        texture_u = hittable -> ceil_border_t -> width - texture_u - 1;
      }

      
      texture_step = hittable -> ceil_border_t -> height / wall_line_len;
      
      texture_pos = (
        hittable -> ceil_border_t -> height 
          - ( line_y2 - line_y1 ) * texture_step
      ); 

     
      for ( v = line_y1; v < line_y2; ++v )
      {
        texture_v = static_cast< int32_t >( texture_pos );
        texture_pos += texture_step;

        color = hittable -> ceil_border_t -> pixels[
          hittable -> ceil_border_t -> width * texture_v + texture_u
        ];

        if ( side == 1 )  color = ( color >> 1 ) & 0x7F7F7F7Fu;

        *( buffer + u + v * buff_w ) = color;
      }
      
    
      
      
      
      //
      //  FLOOR TOP SIDE
      //
      wall_line_len = basic_wall_line_len * old_hittable -> floor_height;
      
      line_y2 = top_line_mask;
      line_y1 = DowngradeType< int16_t >( std::round (
        buff_h / 2.0 - wall_line_len 
          + basic_wall_line_len * ( position_z + 0.5 )
      ) );

      if ( line_y1 < bottom_line_mask )
      {
        line_y1 = bottom_line_mask;
      }
      if ( top_line_mask > line_y1 )
      {
        top_line_mask = line_y1;
      }

      act_distance_correction = (
        2.0 * position_z - 2.0 * old_hittable -> floor_height + 1.0
      );
      for ( v = line_y1; v < line_y2; ++v )
      {
        current_hit_distance = buff_h / ( 2.0 * v - buff_h );
        current_hit_distance *= act_distance_correction;
        weight = current_hit_distance / hit_distance;
        current_hit_pos_x = weight * hit_pos_x + ( 1.0 - weight ) * position_x;
        current_hit_pos_y = weight * hit_pos_y + ( 1.0 - weight ) * position_y;
        current_hit_pos_x -= static_cast< int64_t >( current_hit_pos_x );
        current_hit_pos_y -= static_cast< int64_t >( current_hit_pos_y );

        texture_u = current_hit_pos_x * old_hittable -> floor_top_t -> width;
        texture_v = current_hit_pos_y * old_hittable -> floor_top_t -> height;

        color = old_hittable -> floor_top_t -> pixels[
          old_hittable -> floor_top_t -> width * texture_v + texture_u
        ];

        *( buffer + u + v * buff_w ) = color;
      }
     


      //
      //  FLOOR WALL
      //
      wall_line_len = basic_wall_line_len * hittable -> floor_height;

      //draw hittable border
      line_y2 = line_y1;
      line_y1 = DowngradeType< int16_t >( std::round (
        buff_h / 2.0 - wall_line_len 
          + basic_wall_line_len * ( position_z + 0.5 )
      ) );

      unmusk_line_y = line_y1;
      
      
      if ( top_line_mask < line_y2 )
      {
        line_y2 = top_line_mask;
      }
      if ( line_y1 < bottom_line_mask )
      {
        line_y1 = bottom_line_mask;
      }
      
      if ( top_line_mask > line_y1 )
      {
        top_line_mask = line_y1;
      }


      texture_u = DowngradeType< int32_t >(
        local_hit_pos * hittable -> floor_border_t -> width
      );
      
      
      if ( side == 0  &&  ray_dir_x > 0 ) 
      {
        texture_u = hittable -> floor_border_t -> width - texture_u - 1;
      }
      
      if ( side == 1  &&  ray_dir_y < 0 )
      {
        texture_u = hittable -> floor_border_t -> width - texture_u - 1;
      }

      
      texture_step = hittable -> floor_border_t -> height / wall_line_len;

      texture_pos = ( line_y1 - unmusk_line_y ) * texture_step;

     
      for ( v = line_y1; v < line_y2; ++v )
      {
        texture_v = static_cast< int32_t >( texture_pos );
        texture_pos += texture_step;

        color = hittable -> floor_border_t -> pixels[
          hittable -> floor_border_t -> width * texture_v + texture_u
        ];

        if ( side == 1 )  color = ( color >> 1 ) & 0x7F7F7F7Fu;

        *( buffer + u + v * buff_w ) = color;
      }




      if ( hittable -> floor_height >= max_see_through_height )  break;
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
