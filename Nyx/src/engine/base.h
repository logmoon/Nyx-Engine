# pragma once

# include <inttypes.h>

//////////////////////////
// OS CHECKS
# if defined (_WIN32)
# define OS_WINDOWS
# elif defined (__APPLE__) && defined (__MACH__)
# define OS_MAC
# elif defined (__linux__)
# if defined (__gnu_linux__)
# define OS_LINUX
# elif defined (__ANDROID__)
# define OS_ANDROID
# else
# error system (base of linux kernel) not supported
# endif
# endif

// TODO: Maybe CPU ARCHITECTURE


//////////////////////////
// Helper Marcos

// Asserting stuff??
// # include <assert.h>

#ifdef _MSC_VER
	#define N_INLINE __forceinline
#else
	#define N_INLINE inline __attribute__((always_inline))
#endif


# define EvalPrintI(x) printf("%s = %d\n", #x, (x))
# define EvalPrintLL(x) printf("%s = %lld\n", #x, (x))
# define EvalPrintU(x) printf("%s = %u\n", #x, (x))
# define EvalPrintULL(x) printf("%s = %llu\n", #x, (x))
# define EvalPrintF(x) printf("%s = %f\n", #x, (x))
# define EvalPrintD(x) printf("%s = %lf\n", #x, (x))


# include <string.h>
// Zeros out all each of the first (z) characters of p
# define MemoryZero(p,z) memset((p), 0, (z))
# define MemoryZeroStruct(p) MemoryZero(p, sizeof(*(p)))
# define MemoryZeroArray(p) MemoryZero(p, sizeof(p))

// Compares the first (z) bytes of the objects pointed to by (a) and (b)
# define MemoryMatch(a,b,z) (memcmp((a),(b),(z)) == 0)

// Copies (z) characters from the object pointed to by (s) to the object pointed to by (d)
# define MemoryCopy(d,s,z) memmove((d),(s),(z))
# define MemoryCopyStruct(d,s) MemoryCopy(d,s,Min(sizeof(*(d)), sizeof(*(s))))
# define MemoryCopyArray(d,s) MemoryCopy(d,s,Min(sizeof(d), sizeof(s)))




//////////////////////////
// TYPES
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

#if !defined(__cplusplus)
	#if (defined(_MSC_VER) && _MSC_VER < 1800) || (!defined(_MSC_VER) && !defined(__STDC_VERSION__))
		#ifndef true
		#define true  (0 == 0)
		#endif
		#ifndef false
		#define false (0 != 0)
		#endif
		typedef unsigned char bool;
	#else
		#include <stdbool.h>
	#endif
#endif

typedef struct vector2_float
{
	f32 x;
	f32 y;

}V2f;

typedef struct vector2_int
{
	i32 x;
	i32 y;

}V2i;

typedef struct rect
{
	int x;
	int y;
	int w;
	int h;

}Rect;

//////////////////////////
// Math
# include <math.h>

# define Cos(radians) cosf(radians)
# define Sin(radians) sinf(radians)
# define Sqrt(a) sqrtf(a)
# define Abs(a) ((a) < 0 ? -(a) : (a))
# define Sign(a) (a < 0 ? -1.0f : 1.0f)
N_INLINE void SinCos(float radians, float *s, float *c) { *c = Cos(radians) ; *s = Sin(radians); }

# define Min(a,b) (((a)<(b)) ? (a):(b))
# define Max(a,b) (((a)>(b)) ? (a):(b))
# define Clamp(a,lo,hi) (((lo)<(a)) ? (a) : ((hi)<(lo)) ? (hi):(lo))
# define IsPowerOfTwo(x) ((x & (x-1)) == 0)

// TODO: Vector2 operations (https://github.com/RandyGaul/cute_headers/blob/master/cute_c2.h)
