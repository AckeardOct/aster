#pragma once

#include <entt/entity/registry.hpp>

class SDL_Renderer;

class IRenderSys {
public:
    virtual ~IRenderSys() = default;

    virtual void update(entt::registry& reg, SDL_Renderer& sdl_renderer) = 0;
    bool isEnabled() { return !isPaused; }
    void enable() { isPaused = false; }
    void disable() { isPaused = true; }

private:
    bool isPaused = false;
};

class RectRendSys : public IRenderSys {
public: // IRenderSys interface
    virtual void update(entt::registry& reg, SDL_Renderer& sdl_renderer) override;
};

class PolyRendSys : public IRenderSys {
public: // IRenderSys interface
    void update(entt::registry& reg, SDL_Renderer& sdl_renderer) override;
};
