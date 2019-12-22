#include "cmp_basic.h"

#include "common/logger.h"

#include <Box2D/Collision/Shapes/b2PolygonShape.h>
#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Dynamics/b2Fixture.h>

PositionCmp::PositionCmp(Vec2f pos, Vec2f size, Vec2f direction)
    : pos(pos)
    , size(size)
    , direction(direction)
{
}

MoveCmp::MoveCmp(Vec2f speed)
    : speed(speed)
{
}

PhysDynamicBodyCmp::PhysDynamicBodyCmp(b2World& physWorld, Vec2f pos, Vec2f size)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(pos.x(), pos.y());
    this->body = physWorld.CreateBody(&bodyDef);

    this->body->SetFixedRotation(true);

    b2PolygonShape dynamicBox;
    size /= 2.f;
    dynamicBox.SetAsBox(size.x(), size.y());

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.f;
    fixtureDef.friction = 0.3f;

    this->body->CreateFixture(&fixtureDef);
}

PhysStaticBodyCmp::PhysStaticBodyCmp(b2World& physWorld, Vec2f pos, Vec2f size)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.position.Set(pos.x(), pos.y());
    this->body = physWorld.CreateBody(&bodyDef);

    b2PolygonShape dynamicBox;
    size /= 2.f;
    dynamicBox.SetAsBox(size.x(), size.y());

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;

    this->body->CreateFixture(&fixtureDef);
}
