#include <nlohmann/json.hpp>
#include <LicoriceEngine/World.hpp>




namespace LicEngine
{


void
from_json
(
    const nlohmann::json & j,
    Hittable &             h
)
{
  j . at( "floor_border_index" ) . get_to( h . floor_border_index );
  j . at( "floor_top_index" )    . get_to( h . floor_top_index );
  j . at( "ceil_border_index" )  . get_to( h . ceil_border_index );
  j . at( "ceil_bottom_index" )  . get_to( h . ceil_bottom_index );
  j . at( "floor_height" )       . get_to( h . floor_height );
  j . at( "floor_z" )            . get_to( h . floor_z );
  j . at( "ceil_height" )        . get_to( h . ceil_height );
  j . at( "ceil_z" )             . get_to( h . ceil_z );
}


}
