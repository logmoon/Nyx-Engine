# pragma once

# include <inttypes.h>
# include <stdbool.h>

// Unsigned int types
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

// Signed int types
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

// Floating numbers
typedef float f32;
typedef double f64;

// Math
typedef struct vector2
{
	f32 x;
	f32 y;
}Vector2;
