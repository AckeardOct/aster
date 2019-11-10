#pragma once

#include <entt/entity/registry.hpp>

class IBasicSys {
public:
    virtual void update(entt::registry& reg, float dt) = 0;
    bool isEnabled() { return !isPaused; }
    void enable() { isPaused = false; }
    void disable() { isPaused = true; }

private:
    bool isPaused = false;
};
