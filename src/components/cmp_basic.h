#pragma once

#include <Box2D/Dynamics/b2World.h>
#include <glm/vec2.hpp>

class b2Body;

struct PositionCmp {
    glm::vec2 pos;
    glm::vec2 size;
    glm::vec2 direction;

public:
    PositionCmp(glm::vec2 pos, glm::vec2 size, glm::vec2 direction = glm::vec2(0.f, 0.f));
};

struct MoveCmp {
    glm::vec2 speed;

public:
    MoveCmp() = delete;
    MoveCmp(glm::vec2 speed);

    void move(PositionCmp& posCmp, float dt) { move(posCmp, posCmp.direction, dt); }
    void move(PositionCmp& posCmp, glm::vec2& direction, float dt);
};

struct InputableCmp {
};

struct PhysDynamicBodyCmp {
    b2Body* body = nullptr;

public:
    PhysDynamicBodyCmp(b2World& physWorld, glm::vec2 pos, glm::vec2 size);
};

struct PhysStaticBodyCmp {
    b2Body* body = nullptr;

public:
    PhysStaticBodyCmp(b2World& physWorld, glm::vec2 pos, glm::vec2 size);
};
