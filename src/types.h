#ifndef CBOT_TYPES
#define CBOT_TYPES
#include <stdint.h>

typedef double f64;
typedef float f32;
typedef uint32_t u32;
typedef int32_t b32;
typedef uint64_t u64;
typedef uint8_t u8;

struct triple_f{
    f32 x; 
    f32 y;
    f32 z;
};

struct triple_d{
    f64 x; 
    f64 y;
    f64 z;
};

struct tuple_f{
    f32 x;
    f32 y;
};

struct tuple_d{
    f64 x;
    f64 y;
};

struct tuple_i{
    int x;
    int y;
};

#endif
