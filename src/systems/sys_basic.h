#pragma once

#include <SDL2/SDL_events.h>
#include <entt/entity/registry.hpp>
#include <unordered_map>
#include <vector>

class IBasicSys {
public:
    virtual void update(entt::registry& reg, float dt) = 0;
    bool isEnabled() { return !isPaused; }
    void enable() { isPaused = false; }
    void disable() { isPaused = true; }

private:
    bool isPaused = false;
};

class InputSys : public IBasicSys {
public:
    void onInput(const SDL_Event& event);

public: // IBasicSys interface
    virtual void update(entt::registry& reg, float dt) override;

private:
    std::vector<SDL_Event> events;
    std::unordered_map<int, bool> keys;
};
