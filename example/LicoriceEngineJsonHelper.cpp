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
  j . at( "floor_border" ) . get_to( s . floor_border );
  j . at( "floor_top" )    . get_to( s . floor_top );
  j . at( "ceil_border" )  . get_to( s . ceil_border );
  j . at( "ceil_bottom" )  . get_to( s . ceil_bottom );
  j . at( "floor_height" ) . get_to( s . floor_height );
  j . at( "floor_z" )      . get_to( s . floor_z );
  j . at( "ceil_height" )  . get_to( s . ceil_height );
  j . at( "ceil_z" )       . get_to( s . ceil_z );
}


void
from_json
(
    const nlohmann::json & j,
    Portal &               p
)
{
  j . at( "target_y" ) . get_to( p . target_y );
  j . at( "target_x" ) . get_to( p . target_x );
  j . at( "floor_z" )  . get_to( p . floor_z );
  j . at( "ceil_z" )   . get_to( p . ceil_z );
}


}
