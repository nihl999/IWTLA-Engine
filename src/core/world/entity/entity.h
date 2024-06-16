#pragma once
#include <core/world/entity/components/has_transform.h>

class Entity : public HasTransform
{
    u32 id;

public:
    virtual ~Entity() {}
};