#include "cmp_basic.h"

#include "common/logger.h"

#include <Box2D/Collision/Shapes/b2PolygonShape.h>
#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Dynamics/b2Fixture.h>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

PositionCmp::PositionCmp(Vec2f pos, Vec2f size)
{
    trans.position.x = pos.x;
    trans.position.y = pos.y;

    trans.sizeScale.x = size.x;
    trans.sizeScale.y = size.y;
}

MoveCmp::MoveCmp(Vec2f speed)
    : speed(speed)
{
}

PhysDynamicBodyCmp::PhysDynamicBodyCmp(b2World& physWorld, const PositionCmp& posCmp)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(posCmp.trans.position.x, posCmp.trans.position.y);
    body = physWorld.CreateBody(&bodyDef);

    this->body->SetFixedRotation(true);

    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(posCmp.trans.sizeScale.x / 2.f, posCmp.trans.sizeScale.y / 2.f);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.friction = 0.f;
    fixtureDef.restitution = 1.0f;

    const Vec3f& euler = glm::eulerAngles(posCmp.trans.orientation);
    body->SetTransform(b2Vec2(posCmp.trans.position.x, posCmp.trans.position.y), -euler.z);
    body->CreateFixture(&fixtureDef);
}

PhysStaticBodyCmp::PhysStaticBodyCmp(b2World& physWorld, const PositionCmp& posCmp)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.position.Set(posCmp.trans.position.x, posCmp.trans.position.y);
    this->body = physWorld.CreateBody(&bodyDef);

    b2PolygonShape staticBox;
    staticBox.SetAsBox(posCmp.trans.sizeScale.x / 2.f, posCmp.trans.sizeScale.y / 2.f);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &staticBox;

    const Vec3f& euler = glm::eulerAngles(posCmp.trans.orientation);
    body->SetTransform(b2Vec2(posCmp.trans.position.x, posCmp.trans.position.y), -euler.z);
    body->CreateFixture(&fixtureDef);
}
