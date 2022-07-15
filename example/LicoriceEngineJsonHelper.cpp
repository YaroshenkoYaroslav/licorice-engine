#include <nlohmann/json.hpp>
#include <LicoriceEngine/World.hpp>




namespace LicEngine
{


void
from_json
(
    const nlohmann::json & j,
    Shape &                s
)
{
  j . at( "floor_border_index" ) . get_to( s . floor_border_index );
  j . at( "floor_top_index" )    . get_to( s . floor_top_index );
  j . at( "ceil_border_index" )  . get_to( s . ceil_border_index );
  j . at( "ceil_bottom_index" )  . get_to( s . ceil_bottom_index );
  j . at( "floor_height" )       . get_to( s . floor_height );
  j . at( "floor_z" )            . get_to( s . floor_z );
  j . at( "ceil_height" )        . get_to( s . ceil_height );
  j . at( "ceil_z" )             . get_to( s . ceil_z );
}


}
