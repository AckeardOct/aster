#pragma once

#include "common/math_utils.h"

#include <Box2D/Dynamics/b2World.h>

class b2Body;

struct PositionCmp {
    Vec2f pos;
    Vec2f size;

    Transform trans;

public:
    PositionCmp(Vec2f pos, Vec2f size);
};

struct MoveCmp {
    Vec2f speed;

public:
    MoveCmp(Vec2f speed);
};

struct InputableCmp {
};

struct PhysDynamicBodyCmp {
    b2Body* body = nullptr;

public:
    PhysDynamicBodyCmp(b2World& physWorld, Vec2f pos, Vec2f size);
};

struct PhysStaticBodyCmp {
    b2Body* body = nullptr;

public:
    PhysStaticBodyCmp(b2World& physWorld, Vec2f pos, Vec2f size);
};
