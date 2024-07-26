#pragma once

#include <core/defines.h>
#include <string>

// struct application_spec {
//   std::string application_name;
//   u32 width;
//   u32 height;
//   u32 x;
//   u32 y;
// };

// Opaque platform indepependent state - could be typedef void* state
struct platform_state {
    void *internal_state;
};

// Platform specific start and shutdown
bool platform_init(platform_state *state, std::string application_name,
                   u32 width, u32 height, u32 x, u32 y);

bool platform_shutdown(platform_state *state);

bool platform_pump_messages(platform_state *plat_state);

// Platform specific memory manipulation
void *platform_allocate(u64 size, bool aligned);
void platform_free(void *block, bool aligned);
void *platform_zero_memory(void *block, u64 size);
void *platform_copy_memory(void *dest, const void *source, u64 size);
void *platform_set_memory(void *block, i32 value, u64 size);

// NOTE: On kohi engine, this had a platform console write and write error. I'm
// third partying to spdlog

f64 platform_get_absolute_time();

void platform_sleep(u64 ms);