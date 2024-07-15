#include <core/utils/uuid/uuid.h>
#include <random>

namespace Random
{
    // NOTE: Hazel Engine UUID - stolen...
    std::random_device s_RandomDevice;
    std::mt19937_64 s_Engine(s_RandomDevice());
    std::uniform_int_distribution<u64> s_UniformDistribution;
    std::string GenerateStringUUID()
    {
        return std::to_string(GenerateUUID());
    }
    UUID GenerateUUID()
    {
        return s_UniformDistribution(s_Engine);
    }
}