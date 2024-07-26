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

#ifdef _DEBUG
#define OURO_DEBUG
#else
#define OURO_RELEASE
#endif

// Platform detection - KOHI Engine
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#define OURO_PLATFORM_WINDOWS 1
#ifndef _WIN64
#error "Only 64-Bit Windows supported"
#endif
#elif defined(__linux__) || defined(__gnu_linux__)
#define OURO_PLATFORM_LINUX 1
#if defined(__ANDROID__)
#define OURO_PLATFORM_ANDROID 1
#endif
#elif defined(__unix_)
#endif
// TODO: More platforms in detection - Unix, MacOS, iOS, Posix
// I currently won't support Linux, only detect it.

#ifdef OURO_PLATFORM_WINDOWS
#ifdef OURO_EXPORT
#define OUROAPI __declspec(dllexport)
#else
#define OUROAPI __declspec(dllimport)
#endif
#endif

#ifdef OURO_PLATFORM_LINUX
#ifdef OURO_EXPORT
#define OUROAPI __attribute__((visibility("default")))
#else
#define OUROAPI
#endif
#endif
