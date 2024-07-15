#pragma once
#include <core/defines.h>
#include <string>

namespace Random
{
    typedef u64 UUID;

    UUID GenerateUUID();
    std::string GenerateStringUUID();
}