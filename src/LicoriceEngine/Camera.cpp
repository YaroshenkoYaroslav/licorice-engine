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
  
  int16_t   cline_y1;
  int16_t   cline_y2;
  int16_t   fline_y1;
  int16_t   fline_y2;

  int32_t   texture_u;
  int32_t   texture_v;

  int32_t   map_x;
  int32_t   map_y;

  Uint32    color;

  double    basic_wall_line_len;
  double    wall_line_len;
  
  double    camera_x;
  
  double    ray_dir_x;
  double    ray_dir_y;
  double    ray_z;
  
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
  
  double    basic_buff_distance  [ buff_h ];



  const Hittable *  hittable;
  const Texture  *  texture;
  const Shape *     shape;
  



  for ( v = 0; v < buff_h; ++v )
  {
    basic_buff_distance[ v ] = buff_h / ( buff_h - 2.0 * v );
  }


  
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
      
    hittable = & world . map[ map_x + map_y * world.map_width ];
    shape = & world . shapes [ hittable -> index ];

    ray_z = position_z;

    while ( true )
    {
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
      if ( side == 0 )  local_hit_pos = hit_pos_y;
      else              local_hit_pos = hit_pos_x;
      local_hit_pos -= static_cast< int64_t >( local_hit_pos );
      
      basic_wall_line_len = hit_distance ? buff_h / hit_distance : 1e30;
      

      //
      //  CEIL BOTTOM FACE
      //
      wall_line_len = basic_wall_line_len * shape -> ceil_height;
      
      cline_y1 = bottom_line_mask;
      cline_y2 = DowngradeType< int16_t >( std::round (
        buff_h / 2.0 + wall_line_len
          - basic_wall_line_len * ( shape -> ceil_z - ray_z - 0.5 )
      ) );

      if ( cline_y2 > top_line_mask )
      {
        cline_y2 = top_line_mask;
      }
      if ( bottom_line_mask < cline_y2 )
      {
        bottom_line_mask = cline_y2;
      }

      act_distance_correction = (
        shape -> ceil_z - ray_z - shape -> ceil_height  - 0.5
      ) * 2.0;

      texture = & world . textures [ shape -> ceil_bottom ];
      
      for ( v = cline_y1; v < cline_y2; ++v )
      {
        current_hit_distance = (
          basic_buff_distance[ v ] * act_distance_correction
        );
        weight = current_hit_distance / hit_distance;
        current_hit_pos_x = weight * ( hit_pos_x - position_x) + position_x;
        current_hit_pos_y = weight * ( hit_pos_y - position_y) + position_y;
        current_hit_pos_x -= static_cast< int64_t >( current_hit_pos_x );
        current_hit_pos_y -= static_cast< int64_t >( current_hit_pos_y );

        texture_u = std::abs( current_hit_pos_x * texture -> width );
        texture_v = std::abs( current_hit_pos_y * texture -> height );

        color = texture -> pixels[ texture -> width * texture_v + texture_u ];

        *( buff_w * v + u + buffer ) = color;
      }

      
      //
      //  FLOOR TOP SIDE
      //
      wall_line_len = basic_wall_line_len * shape -> floor_height;
      
      fline_y2 = top_line_mask;
      fline_y1 = DowngradeType< int16_t >( std::round (
        buff_h / 2.0 - wall_line_len 
          + basic_wall_line_len * ( ray_z + 0.5 - shape -> floor_z )
      ) );

      if ( fline_y1 < bottom_line_mask )
      {
        fline_y1 = bottom_line_mask;
      }
      if ( top_line_mask > fline_y1 )
      {
        top_line_mask = fline_y1;
      }

      act_distance_correction = (
         ray_z - shape -> floor_z - shape -> floor_height + 0.5
      ) * 2.0;

      
      texture = & world . textures [ shape -> floor_top ];
      
      for ( v = fline_y1; v < fline_y2; ++v )
      {
        current_hit_distance = (
          -basic_buff_distance[ v ] * act_distance_correction
        );
        weight = current_hit_distance / hit_distance;
        current_hit_pos_x = weight * ( hit_pos_x - position_x) + position_x;
        current_hit_pos_y = weight * ( hit_pos_y - position_y) + position_y;
        current_hit_pos_x -= static_cast< int64_t >( current_hit_pos_x );
        current_hit_pos_y -= static_cast< int64_t >( current_hit_pos_y );

        texture_u = std::abs( current_hit_pos_x * texture -> width );
        texture_v = std::abs( current_hit_pos_y * texture -> height );

        color = texture -> pixels[ texture -> width * texture_v + texture_u ];

        *( buff_w * v + u + buffer ) = color;
      }
      

 

      if
      (
        map_x < 0 || map_x >= world.map_width
        ||
        map_y < 0 || map_y >= world.map_height
        ||
        bottom_line_mask == top_line_mask
      )
      {
        break;
      }

      hittable = & world . map[ map_x + map_y * world.map_width ];
      
      if ( hittable -> m_type == Hittable::Type::Portal )
      {
        map_x = world . portals[ hittable -> index ] . target_x;
        map_y = world . portals[ hittable -> index ] . target_y;
        hittable = & world . map[ map_x + map_y * world.map_width ];
        shape = & world . shapes [ hittable -> index ];
        ray_z = shape -> floor_z + shape -> floor_height;
      }
      else
      {
        shape = & world . shapes [ hittable -> index ];
      }



      
      //
      //  CEIL WALL
      //
      wall_line_len = basic_wall_line_len * shape -> ceil_height;

      cline_y1 = cline_y2;
      cline_y2 = DowngradeType< int16_t >( std::round (
        buff_h / 2.0 + wall_line_len
          - basic_wall_line_len * ( shape -> ceil_z - ray_z - 0.5 )
      ) );
      
      unmusk_line_y = cline_y2;
      
      
      if ( bottom_line_mask > cline_y1 )
      {
        cline_y1 = bottom_line_mask;
      }
      if ( cline_y2 > top_line_mask )
      {
        cline_y2 = top_line_mask;
      }
      
      if ( bottom_line_mask < cline_y2 )
      {
        bottom_line_mask = cline_y2;
      }


      texture = & world . textures [ shape -> ceil_border ];
      
      texture_u = DowngradeType< int32_t >( local_hit_pos * texture -> width );
      
      
      if ( side == 0  &&  ray_dir_x > 0 ) 
      {
        texture_u = texture -> width - texture_u - 1;
      }
      
      if ( side == 1  &&  ray_dir_y < 0 )
      {
        texture_u = texture -> width - texture_u - 1;
      }

      
      texture_step = texture -> height / wall_line_len;
     
      texture_pos = (
        texture -> height - ( unmusk_line_y - cline_y1 ) * texture_step
      ); 
      if ( texture_pos < 0 )  texture_pos = 0;

    

      for ( v = cline_y1; v < cline_y2; ++v )
      {
        texture_v = static_cast< int32_t >( texture_pos );
        texture_pos += texture_step;

        color = texture -> pixels[ texture -> width * texture_v + texture_u ];

        if ( side == 1 )  color = ( color >> 1 ) & 0x7F7F7F7Fu;

        *( buff_w * v + u + buffer ) = color;
      }
      
    
      
      
     


      //
      //  FLOOR WALL
      //
      wall_line_len = basic_wall_line_len * shape -> floor_height;

      fline_y2 = fline_y1;
      fline_y1 = DowngradeType< int16_t >( std::round (
        buff_h / 2.0 - wall_line_len 
          + basic_wall_line_len * ( ray_z + 0.5 - shape -> floor_z )
      ) );

      unmusk_line_y = fline_y1;
      
      
      if ( top_line_mask < fline_y2 )
      {
        fline_y2 = top_line_mask;
      }
      if ( fline_y1 < bottom_line_mask )
      {
        fline_y1 = bottom_line_mask;
      }
      
      if ( top_line_mask > fline_y1 )
      {
        top_line_mask = fline_y1;
      }

      texture = & world . textures [ shape -> floor_border ];

      texture_u = DowngradeType< int32_t >(
        local_hit_pos * texture -> width
      );
      
      
      if ( side == 0  &&  ray_dir_x > 0 ) 
      {
        texture_u = texture -> width - texture_u - 1;
      }
      
      if ( side == 1  &&  ray_dir_y < 0 )
      {
        texture_u = texture -> width - texture_u - 1;
      }

      
      texture_step = texture -> height / wall_line_len;

      texture_pos = ( fline_y1 - unmusk_line_y ) * texture_step;

           
      for ( v = fline_y1; v < fline_y2; ++v )
      {
        texture_v = static_cast< int32_t >( texture_pos );
        texture_pos += texture_step;

        color = texture -> pixels[ texture -> width * texture_v + texture_u ];

        if ( side == 1 )  color = ( color >> 1 ) & 0x7F7F7F7Fu;

        *( buff_w * v + u + buffer ) = color;
      }
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
