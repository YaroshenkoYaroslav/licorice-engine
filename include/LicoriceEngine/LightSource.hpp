#ifndef LICORICE_ENGINE_LIGHT_SOURCE_HPP
#define LICORICE_ENGINE_LIGHT_SOURCE_HPP

#include <cstdint>
#include <cmath>

namespace LicEngine
{

class LightSource
{
 public:
  
  virtual
  void
  Use
  (
      double *  light_map,
      int32_t   map_width,
      int32_t   map_height
  )
  const
  = 0;
};

// TODO fix virtual
class LightPoint
  : virtual public LightSource
{
 public:
  
  void
  Use
  (
      double *  light_map,
      int32_t   map_width,
      int32_t   map_height
  )
  const
  override;

 public:

  int32_t  position_x;
  int32_t  position_y;
  int32_t  radius;
  double   intensity;
};

class LightArea
  : virtual public LightSource
{
 public:
  
  void
  Use
  (
      double *  light_map,
      int32_t   map_width,
      int32_t   map_height
  )
  const
  override;

 public:

  int32_t  x1;
  int32_t  y1;
  int32_t  x2;
  int32_t  y2;
  double   intensity;
};

}

#endif
