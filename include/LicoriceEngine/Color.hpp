#ifndef LICORICE_ENGINE_COLOR_HPP
#define LICORICE_ENGINE_COLOR_HPP

#include <cstdint>

namespace LicEngine :: Color
{


using Uint32  =  uint32_t;
using Uint8   =  uint8_t;


inline 
Uint32
Mix
(
    const Uint32  color1, 
    const Uint32  color2,
    const double  x
);

inline 
Uint32
RGBAtoUINT32
(
    const Uint8  r,
    const Uint8  g,
    const Uint8  b,
    const Uint8  a
);

inline 
Uint8
GetR
(
    const Uint32  color
);

inline 
Uint8
GetG
(
    const Uint32  color
);

inline 
Uint8
GetB
(
    const Uint32  color
);

inline 
Uint8
GetA
(
    const Uint32  color
);


#include "Color.ini"


}

// more fast for render [ Mix ( 0, color, x ) ]
#define COLOR_FAST_MIX_BLACK( color , x ) (\
    Color::Uint32((x)*(((color)>>24)&0x000000FF))<<24  \
  | Color::Uint32((x)*(((color)>>16)&0x000000FF))<<16  \
  | Color::Uint32((x)*(((color)>>8)&0x000000FF))<<8    \
  | (color)&0x000000FF                                 \
)
  

#endif
