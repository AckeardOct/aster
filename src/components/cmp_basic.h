#pragma once

#include <glm/vec2.hpp>

struct PositionCmp {
    glm::vec2 pos;
    glm::vec2 size;
    glm::vec2 direction;

public:
    PositionCmp(glm::vec2 pos, glm::vec2 size, glm::vec2 direction = glm::vec2(0.f, 0.f))
        : pos(pos)
        , size(size)
        , direction(direction)
    {
    }
};
