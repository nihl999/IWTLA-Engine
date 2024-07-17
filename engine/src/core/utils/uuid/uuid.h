#pragma once
#include <core/defines.h>
#include <string>

namespace Random
{
    typedef u64 UUID;

    UUID OUROAPI GenerateUUID();
    std::string OUROAPI GenerateStringUUID();
}