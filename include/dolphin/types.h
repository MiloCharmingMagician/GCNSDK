/*---------------------------------------------------------------------------*
  Project:  Dolphin OS
  File:     types.h
 *---------------------------------------------------------------------------*/

#ifndef TYPES_H_
#define TYPES_H_

#ifdef  __MWERKS__
typedef signed char         s8;
typedef signed short        s16;
typedef signed long         s32;
typedef signed long long    s64;
typedef unsigned char       u8;
typedef unsigned short      u16;
typedef unsigned long       u32;
typedef unsigned long long  u64;
#else       // __MWERKS__
#ifdef X86  // GCC
typedef unsigned long long  u64;
typedef   signed long long  s64;
typedef unsigned int        u32;
typedef   signed int        s32;
typedef unsigned short      u16;
typedef   signed short      s16;
typedef unsigned char       u8;
typedef   signed char       s8;
#else
#ifdef __SN__               // for SN
typedef   signed char       s8;
typedef   signed short      s16;
typedef   signed long       s32;
typedef   signed long long  s64;
typedef unsigned char       u8;
typedef unsigned short      u16;
typedef unsigned long       u32;
typedef unsigned long long  u64;
#else // __SN__
// win32 style
typedef __int8              s8;
typedef __int16             s16;
typedef __int32             s32;
typedef __int64             s64;
typedef unsigned __int8     u8;
typedef unsigned __int16    u16;
typedef unsigned __int32    u32;
typedef unsigned __int64    u64;
#endif      // __SN__
#endif      // X86
#endif      // __MWERKS__

typedef volatile u8         vu8;
typedef volatile u16        vu16;
typedef volatile u32        vu32;
typedef volatile u64        vu64;
typedef volatile s8         vs8;
typedef volatile s16        vs16;
typedef volatile s32        vs32;
typedef volatile s64        vs64;

typedef float               f32;
typedef double              f64;
typedef volatile f32        vf32;
typedef volatile f64        vf64;

#ifndef BOOL
typedef int                 BOOL;
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef NULL
#ifdef  __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif

#define DOLPHIN_ALIGNMENT 32

#define ATTRIBUTE_ALIGN(num) __attribute__ ((aligned (num)))

#ifdef __SN__
#define AT_ADDRESS(xyz) __attribute__((address((xyz))))
#else
#define AT_ADDRESS(xyz) : (xyz)
#endif // __SN__
#endif  // TYPES_H_
