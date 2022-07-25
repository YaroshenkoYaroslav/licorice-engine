#include <LicoriceEngine/LightSource.hpp>

namespace LicEngine
{

void
LightPoint::Use
(
    double *  light_map,
    int32_t   map_width,
    int32_t   map_height

)
const
{
  double   light_step;
  double   size;
  int32_t  x;
  int32_t  y;
  int32_t  index;

 

  // TODO improve the code ( now 0(n) but bad code )
  
  index = position_y * map_width + position_x;
  light_map[ index ] += intensity;
  if ( light_map[ index ] > 1 )  light_map[ index ] = 1;

  for ( size = 1; size <= radius; ++size )
  {
    light_step = ( 1 - size / radius );

    for ( y = position_y - size; y <= position_y + size; y += 2 * size )
    {
      if ( y < 0 || y >= map_height )  continue;
      for
      (
        x = std::max(
          static_cast< int32_t >( position_x - size ), 0
        );
        x <= std::min(
          static_cast< int32_t >( map_width - 1 ),
          static_cast< int32_t >( position_x + size )
        );
        ++x
      )
      {
        index = y * map_width + x;
        light_map[ index ] += intensity * light_step;

        if ( light_map[ index ] > 1 )  light_map[ index ] = 1;
      }
    }
   
    for ( x = position_x - size; x <= position_x + size; x += 2 * size )
    {
      if ( x < 0 || x >= map_width )  continue;
      for
      (
        y = std::max(
          static_cast< int32_t >( position_y - size + 1 ), 0
        );
        y < std::min(
          static_cast< int32_t >( map_height ),
          static_cast< int32_t >( position_y + size )
        );
        ++y 
      )
      {
        index = y * map_width + x;
        light_map[ index ] += intensity * light_step;
        if ( light_map[ index ] > 1 )  light_map[ index ] = 1;
      }
    }
  }
}


void
LightArea::Use
(
    double *  light_map,
    int32_t   map_width,
    int32_t   map_height
)
const
{
  int32_t  x;
  int32_t  y;
  int32_t  index;


  for ( y = y1 ; y <= y2; ++y )
  {
    for ( x = x1 ; x <= x2; ++x )
    {
      index = map_width * y + x;
      light_map[ index ] += intensity;
      if ( light_map[ index ] > 1 )  light_map[ index ] = 1;
    }
  }
}

}
