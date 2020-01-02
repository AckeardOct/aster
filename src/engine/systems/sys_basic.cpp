#include "sys_basic.h"

#include "common/math_utils.h"
#include "components/cmp_basic.h"

#include <Box2D/Dynamics/b2Body.h>
#include <SDL2/SDL_events.h>

void InputSys::onInput(const SDL_Event& event)
{
    switch (event.type) {
    case SDL_KEYDOWN:
        if (false == keys[event.key.keysym.sym]) {
            keys[event.key.keysym.sym] = true;
            events.push_back(event);
        }
        break;
    case SDL_KEYUP:
        if (true == keys[event.key.keysym.sym]) {
            keys[event.key.keysym.sym] = false;
            events.push_back(event);
        }
        break;
    }
}

void InputSys::update(entt::registry& reg, float dt)
{
    Vec2f direction(0, 0);
    for (auto& iter : keys) {
        if (iter.second) {
            switch (iter.first) {
            case SDLK_UP:
                direction.y -= 1.f;
                break;
            case SDLK_DOWN:
                direction.y += 1.f;
                break;
            case SDLK_LEFT:
                direction.x -= 1.f;
                break;
            case SDLK_RIGHT:
                direction.x += 1.f;
                break;
            }
        }
    }

    auto physView = reg.view<MoveCmp, PhysDynamicBodyCmp, InputableCmp>();
    for (auto et : physView) {
        if (isZero(direction)) {
            continue;
        }

        direction = normalize(direction);
        MoveCmp& moveCmp = physView.get<MoveCmp>(et);
        PhysDynamicBodyCmp& bodyCmp = physView.get<PhysDynamicBodyCmp>(et);

        b2Vec2 impulse(direction.x * moveCmp.speed.x, direction.y * moveCmp.speed.y);
        bodyCmp.body->SetLinearVelocity(impulse);
    }

    events.clear();
}

PhysSys::PhysSys(b2World& physWorld)
    : physWorld(physWorld)
{
}

void PhysSys::update(entt::registry& reg, float dt)
{
    auto physView = reg.view<PositionCmp, PhysDynamicBodyCmp>();
    for (auto et : physView) {
        PositionCmp& posCmp = physView.get<PositionCmp>(et);
        PhysDynamicBodyCmp& physBodyCmp = physView.get<PhysDynamicBodyCmp>(et);

        b2Vec2 position = physBodyCmp.body->GetPosition();
        // TODO: maybe check angles in Box2D ???

        posCmp.pos.x = position.x;
        posCmp.pos.y = position.y;
    }
}
