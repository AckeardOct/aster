#include "cmp_basic.h"

PositionCmp::PositionCmp(glm::vec2 pos, glm::vec2 size, glm::vec2 direction)
    : pos(pos)
    , size(size)
    , direction(direction)
{
}

MoveCmp::MoveCmp(glm::vec2 speed)
    : speed(speed)
{
}

void MoveCmp::move(PositionCmp& posCmp, glm::vec2& direction, float dt)
{
    static const float diff = 0.5f;
    glm::vec2 tmpPos = posCmp.pos;
    float dtSecs = dt / 1000.f;
    double stepX = direction.x * dtSecs * speed.x;
    double stepY = direction.y * dtSecs * speed.y;
    posCmp.pos.x += stepX;
    posCmp.pos.y += stepY;
}
