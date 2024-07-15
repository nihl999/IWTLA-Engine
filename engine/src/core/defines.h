#pragma once
#include <stdint.h>

// kohi #001
#if defined(__clang__) || defined(__gcc__)
#define STATIC_ASSERT _Static_assert
#else
#define STATIC_ASSERT static_assert
#endif

// sized types
typedef uint8_t byte;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef float f32;
typedef double f64;

STATIC_ASSERT(sizeof(byte) == 1, "byte should be 1 byte");
STATIC_ASSERT(sizeof(u8) == 1, "u8 should be 1 byte");
STATIC_ASSERT(sizeof(u16) == 2, "u16 should be 2 bytes");
STATIC_ASSERT(sizeof(u32) == 4, "u32 should be 4 bytes");
STATIC_ASSERT(sizeof(u64) == 8, "u64 should be 8 bytes");

STATIC_ASSERT(sizeof(i8) == 1, "i8 should be 1 byte");
STATIC_ASSERT(sizeof(i16) == 2, "i16 should be 2 bytes");
STATIC_ASSERT(sizeof(i32) == 4, "i32 should be 4 bytes");
STATIC_ASSERT(sizeof(i64) == 8, "i64 should be 8 bytes");

STATIC_ASSERT(sizeof(f32) == 4, "f32 should be 4 byte");
STATIC_ASSERT(sizeof(f64) == 8, "f64 should be 8 bytes");

#ifdef OURO_EXPORT_API
#ifdef OURO_PLATFORM_WIN
#define OUROAPI __declspec(dllexport)
#endif
#ifdef OURO_PLATFORM_LINUX
#define OUROAPI __attribute__((visibility("default")))
#endif
#else
#ifdef OURO_PLATFORM_WIN
#define OUROAPI __declspec(dllimport)
#endif
#ifdef OURO_PLATFORM_LINUX
#define OUROAPI
#endif
#endif
