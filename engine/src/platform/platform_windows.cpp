#include "core/logger.h"
#include <core/defines.h>
#ifdef OURO_PLATFORM_WINDOWS

#include <platform/platform.h>

#include <windows.h>
#include <windowsx.h>

// Clock
static f64 clock_frequency;
static LARGE_INTEGER start_time;

struct internal_state {
    HINSTANCE h_instance;
    HWND window_handle;
};

LRESULT CALLBACK process_message(HWND hwnd, u32 msg, WPARAM w_param,
                                 LPARAM l_param);

bool platform_init(platform_state *plat_state, std::string application_name,
                   u32 width, u32 height, u32 x, u32 y) {
    plat_state->internal_state =
        platform_allocate(sizeof(internal_state), true);
    internal_state *state = (internal_state *)plat_state->internal_state;

    state->h_instance = GetModuleHandle(0);

    HICON icon = LoadIcon(state->h_instance, IDI_APPLICATION);
    WNDCLASS window_class;
    platform_zero_memory(&window_class, sizeof(window_class));

    window_class.style = CS_DBLCLKS;
    window_class.lpfnWndProc = process_message;
    window_class.cbClsExtra = 0;
    window_class.cbWndExtra = 0;
    window_class.hInstance = state->h_instance;
    window_class.hIcon = icon;
    window_class.hCursor = LoadCursor(NULL, IDC_ARROW);
    window_class.hbrBackground = NULL;
    window_class.lpszClassName = "ouroboros_window_class";

    if (!RegisterClassA(&window_class)) {
        OUROFATAL("Couldn't initialize Windows platform. Window class creation "
                  "failed!");
        return false;
    }

    // Create window
    u32 client_x = x;
    u32 client_y = y;
    u32 client_width = width;
    u32 client_height = height;

    u32 window_x = client_x;
    u32 window_y = client_y;
    u32 window_width = client_width;
    u32 window_height = client_height;

    u32 window_style = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION;
    u32 window_ex_style = WS_EX_APPWINDOW;

    window_style |= WS_MAXIMIZEBOX;
    window_style |= WS_MINIMIZEBOX;
    window_style |= WS_THICKFRAME;

    // Obtain the size of the border.
    RECT border_rect = {0, 0, 0, 0};
    AdjustWindowRectEx(&border_rect, window_style, 0, window_ex_style);

    // In this case, the border rectangle is negative.
    window_x += border_rect.left;
    window_y += border_rect.top;

    // Grow by the size of the OS border.
    window_width += border_rect.right - border_rect.left;
    window_height += border_rect.bottom - border_rect.top;

    HWND handle = CreateWindowExA(window_ex_style, "ouroboros_window_class",
                                  application_name.c_str(), window_style,
                                  window_x, window_y, window_width,
                                  window_height, 0, 0, state->h_instance, 0);

    if (handle == 0) {

        OUROFATAL("Window creation failed!");
        return false;
    } else {
        state->window_handle = handle;
    }

    // Show the window
    bool should_activate = 1;
    i32 show_window_command_flags =
        should_activate ? SW_SHOW : SW_SHOWNOACTIVATE;
    ShowWindow(state->window_handle, show_window_command_flags);

    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    clock_frequency = 1.0 / (f64)frequency.QuadPart;
    QueryPerformanceCounter(&start_time);

    return true;
}

bool platform_shutdown(platform_state *state) {
    return true;
}

// Platform specific memory manipulation
void *platform_allocate(u64 size, bool aligned) {
    return malloc(size);
}
void platform_free(void *block, bool aligned) {
    free(block);
}

void *platform_zero_memory(void *block, u64 size) {
    return platform_set_memory(block, 0, size);
}
void *platform_copy_memory(void *dest, const void *source, u64 size) {
    return memcpy(dest, source, size);
}
void *platform_set_memory(void *block, i32 value, u64 size) {
    return memset(block, value, size);
}

// NOTE: On kohi engine, this had a platform console write and write error. I'm
// third partying to spdlog

f64 platform_get_absolute_time() {
    LARGE_INTEGER now_time;
    QueryPerformanceCounter(&now_time);
    return (f64)now_time.QuadPart * clock_frequency;
}

void platform_sleep(u64 ms) {
    Sleep(ms);
}

#endif