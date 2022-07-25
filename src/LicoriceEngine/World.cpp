#include <LicoriceEngine/World.hpp>

namespace LicEngine
{

void
World::UpdateLightMap
(

)
{
  int64_t  index;


  for ( index = 0; index < map_width * map_height; ++index )
  {
    light_map[ index ] = luminance;
  }

  for ( index = 0; index < light_sources_count; ++index )
  {
    light_sources[ index ] -> Use(
      light_map,
      map_width,
      map_height
    );
  }
}

}
