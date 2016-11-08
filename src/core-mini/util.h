#pragma once

#include <stdint.h>

// undefine stdlib's abs if encountered
#ifdef abs
#undef abs
#endif

#define PI          3.141592653589793238462643383279
#define HALF_PI     1.570796326794896619231321691639
#define TWO_PI      6.283185307179586476925286766559
#define DEG_TO_RAD  0.017453292519943295769236907684
#define RAD_TO_DEG 57.295779513082320876798154814105
#define EULER       2.718281828459045235360287471352

#define min(a,b)                ((a)<(b)?(a):(b))
#define max(a,b)                ((a)>(b)?(a):(b))
#define abs(x)                  ((x)>0?(x):-(x))
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define round(x)                ((x)>=0?(int32_t)((x)+0.5):(int32_t)((x)-0.5))
#define radians(deg)            ((deg)*DEG_TO_RAD)
#define degrees(rad)            ((rad)*RAD_TO_DEG)
#define sq(x)                   ((x)*(x))
