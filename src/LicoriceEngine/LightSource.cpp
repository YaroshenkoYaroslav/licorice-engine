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
  double   size;
  int32_t  x;
  int32_t  y;

 

  // TODO improve the code ( now 0(n) but bad code )
  
  light_map[ position_y * map_width + position_x ] = intensity;

  for ( size = 1; size <= radius; ++size )
  {
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
          static_cast< int32_t >(position_x + size )
        );
        ++x
      )
      {
        light_map[ y * map_width + x ] += intensity * ( 1 - size / radius );
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
          static_cast< int32_t >( map_height - 1 ),
          static_cast< int32_t >( position_y + size )
        );
        ++y 
      )
      {
        light_map[ y * map_width + x ] += intensity * ( 1 - size / radius );
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


  for ( y = y1 ; y <= y2; ++y )
  {
    for ( x = x1 ; x <= x2; ++x )
    {
      light_map[ map_width * y + x ] += intensity;
    }
  }
}

}
