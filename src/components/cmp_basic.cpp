#include "cmp_basic.h"

#include "../common/logger.h"

#include <Box2D/Collision/Shapes/b2PolygonShape.h>
#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Dynamics/b2Fixture.h>

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

PhysDynamicBodyCmp::PhysDynamicBodyCmp(b2World& physWorld, glm::vec2 pos, glm::vec2 size)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(pos.x, pos.y);
    this->body = physWorld.CreateBody(&bodyDef);

    b2PolygonShape dynamicBox;
    size /= 2.f;
    dynamicBox.SetAsBox(size.x, size.y);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;

    this->body->CreateFixture(&fixtureDef);
}

PhysStaticBodyCmp::PhysStaticBodyCmp(b2World& physWorld, glm::vec2 pos, glm::vec2 size)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.position.Set(pos.x, pos.y);
    this->body = physWorld.CreateBody(&bodyDef);

    b2PolygonShape dynamicBox;
    size /= 2.f;
    dynamicBox.SetAsBox(size.x, size.y);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;

    this->body->CreateFixture(&fixtureDef);
}
