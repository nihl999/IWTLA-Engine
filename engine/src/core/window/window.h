#pragma once

#include <core/defines.h>

class OUROAPI Window {
public:
    int width;
    int height;
    float aspectRatio;

  public:
    Window(int _width, int _height);
    void NewFrame();
    bool ShouldClose();
    void PoolEvents();
    void SwapBuffers();
    ~Window();
};