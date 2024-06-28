#include <core/defines.h>

struct Texture
{
    u32 id;
    i32 width;
    i32 height;
    i8 nrChannels;
    byte *data;
};
