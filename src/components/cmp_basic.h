#pragma once

#include <glm/vec2.hpp>

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
