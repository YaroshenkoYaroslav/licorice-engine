#ifndef LICORICE_ENGINE_WORLD_HPP
#define LICORICE_ENGINE_WORLD_HPP

#include <LicoriceEngine/Color.hpp>
#include <LicoriceEngine/LightSource.hpp>

namespace LicEngine
{

struct Texture
{
  Color::Uint32 *  pixels = nullptr;
  int32_t          width;
  int32_t          height;
};


struct Hittable
{
  enum class Type
  {
    Shape = 0,
    Portal = 1
  };
 

  int32_t  index;
  Type     m_type;
};


struct Shape
{
  int32_t   floor_border;
  int32_t   floor_top;
  int32_t   ceil_border;
  int32_t   ceil_bottom;

  double    floor_height;
  double    floor_z;
  double    ceil_height;
  double    ceil_z;
};

struct Portal
{
  int32_t  target_x;
  int32_t  target_y;
};


class World
{
 public:

  void
  UpdateLightMap
  (
  
  );

 public:
  
  Hittable *      map;
  int32_t         map_width;
  int32_t         map_height;

  Shape *         shapes;
  int32_t         shapes_count;
  
  Portal *        portals;
  int32_t         portals_count;

  Texture *       textures;
  int32_t         textures_count;
  
  
  double *        light_map;
  double          standart_light;
  
  LightSource **  light_sources;
  int32_t         light_sources_count;
};

}

#endif
