namespace Math
{

template < class T > 
inline
int32_t
sign
(
    T  x
)
{
  return ( 0 < x ) - ( x < 0 );
}

}
