#include <LicoriceEngine/Camera.hpp>

namespace LicEngine
{

void
Camera::Render (
    Uint32 * const  buffer,
    int16_t         buff_w,
    int16_t         buff_h, 
    const World &   world
)
{
  int8_t    step_x;
  int8_t    step_y;
  
  int8_t    side;
    
  int16_t   line_mask;
  
  int32_t  x;
  int32_t  y;
  
  int32_t   line_y1;
  int32_t   line_y2;
  
  int32_t   scaled_texture_height;

  int32_t   texture_u;
  int32_t   texture_v;

  int32_t   map_x;
  int32_t   map_y;

  Uint32    color;

  uint16_t  hittable_index;

  double    camera_x;
  
  double    ray_direction_x;
  double    ray_direction_y;
  
  double    delta_distance_x;
  double    delta_distance_y;
  
  double    side_distance_x;
  double    side_distance_y;
  
  double    hit_distance;
  
  double    local_hit_position;

  double    texture_step;
  double    texture_position;


  const Texture *   hittable_texture;
  
  

  for ( x = 0; x < buff_w; ++x )
  {
    camera_x = 2 * x / static_cast< double >( buff_w ) - 1.0;

    ray_direction_x = direction_x + viewing_plane_x * camera_x;
    ray_direction_y = direction_y + viewing_plane_y * camera_x;

    map_x = static_cast< int32_t >( position_x );
    map_y = static_cast< int32_t >( position_y );

    delta_distance_x = std::abs(
      ray_direction_x ? 1 / ray_direction_x : 1e30
    );
    delta_distance_y = std::abs(
      ray_direction_y ? 1 / ray_direction_y : 1e30
    );

    
    if ( ray_direction_x < 0 )
    {
      step_x = -1;
      side_distance_x = ( position_x - map_x ) * delta_distance_x;
    }
    else 
    {
      step_x = 1;
      side_distance_x = ( map_x - position_x + 1 ) * delta_distance_x;
    }
    
    if ( ray_direction_y < 0 )
    {
      step_y = -1;
      side_distance_y = ( position_y - map_y ) * delta_distance_y;
    }
    else 
    {
      step_y = 1;
      side_distance_y = ( map_y - position_y + 1 ) * delta_distance_y;
    }


    
    line_mask = buff_h; 


    while ( true )
    {
      hittable_index = map_x + map_y * world.map_width; 
      hittable_texture = world.map[ hittable_index ].m_texture;

      //move the ray and calculate distance
      if ( side_distance_x < side_distance_y )
      {
        side_distance_x += delta_distance_x;
        map_x += step_x;
        side = 0;
      }
      else
      {
        side_distance_y += delta_distance_y;
        map_y += step_y;
        side = 1;
      }
      
      
      if ( side == 0 )
      {
        hit_distance = ( side_distance_x - delta_distance_x );
      }
      else
      {
        hit_distance = ( side_distance_y - delta_distance_y );
      }
      
      
      
      scaled_texture_height = static_cast< int32_t >(
        hit_distance ? buff_h / hit_distance : 1e30
      );



      //draw hittable top side      
      line_y2 = line_mask;
      line_y1 = static_cast< int32_t >(
        buff_h / 2 + scaled_texture_height * 
          ( 0.5 - world.map[ hittable_index ].height )
      );

      if ( line_y1 < 0 )
      {
        line_y1 = 0;
      }
      
      if ( line_mask > line_y1 )
      {
        line_mask = line_y1;
      }


      for ( int y = line_y1; y < line_y2; ++y )
      {
        *( buffer + x + y * buff_w ) = 0xFFFFFFFF;
      }
     


      hittable_index = map_x + map_y * world.map_width; 
      hittable_texture = world.map[ hittable_index ].m_texture;
      
      

      //draw hittable border
      line_y2 = line_y1;
      line_y1 = static_cast< int32_t >(
        buff_h / 2 + scaled_texture_height * 
          ( 0.5 - world.map[ hittable_index ].height )
      );
     
      
      if ( line_mask < line_y2 )
      {
        line_y2 = line_mask;
      }

      if ( line_y1 < 0 )
      {
        line_y1 = 0;
      }
      
      if ( line_mask > line_y1 )
      {
        line_mask = line_y1;
      }


      
      
      if ( side == 0 )
      {
        local_hit_position = position_y + hit_distance * ray_direction_y;
      }
      else
      {
        local_hit_position = position_x + hit_distance * ray_direction_x;
      }

      local_hit_position -= static_cast< int32_t >(
        local_hit_position
      );
      

      
      texture_u = static_cast< int32_t >(
        local_hit_position * hittable_texture -> width
      );
      
      
      if ( side == 0  &&  ray_direction_x > 0 ) 
      {
        texture_u = hittable_texture -> width - texture_u - 1;
      }
      
      if ( side == 1  &&  ray_direction_y < 0 )
      {
        texture_u = hittable_texture -> width - texture_u - 1;
      }

      
      texture_step = (
        hittable_texture -> height 
          / static_cast< double >( scaled_texture_height )
      );

      texture_position = (
        ( line_y1 + ( scaled_texture_height - buff_h ) / 2 )
          * texture_step
      );

      

      for ( y = line_y1; y < line_y2; ++y )
      {
        texture_v = static_cast< int32_t >( texture_position );
        texture_position += texture_step;

        color = hittable_texture -> pixels[
          hittable_texture -> width * texture_v + texture_u
        ];

        if ( side == 1 )  color = ( color >> 1 ) & 0x7F7F7F7Fu;

        *( buffer + x + y * buff_w ) = color;
      }



      if ( world.map[ hittable_index ].height == 1)   break;
    }
  }
}


void
Camera::Rotate (
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
