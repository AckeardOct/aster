#include "sys_basic.h"

#include "../components/cmp_basic.h"

#include <SDL2/SDL_events.h>
#include <glm/glm.hpp>

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
    glm::vec2 direction(0, 0);
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

    auto moveView = reg.view<PositionCmp, MoveCmp, InputableCmp>();
    for (auto et : moveView) {
        if (direction.length() > 0.1) {
            glm::normalize(direction);
            PositionCmp& posCmp = moveView.get<PositionCmp>(et);
            MoveCmp& moveCmp = moveView.get<MoveCmp>(et);
            moveCmp.move(posCmp, direction, dt);
        }
    }

    events.clear();
}
