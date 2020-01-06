#pragma once

#include <entt/entity/registry.hpp>

#include "render/camera.h"
#include "render/shader.h"

class GameWindow;

class SDL_Renderer;

class IRenderSys {
public:
    IRenderSys(GameWindow& window);
    virtual ~IRenderSys() = default;

    virtual void update(entt::registry& reg, SDL_Renderer& sdl_renderer) = 0;
    bool isEnabled() { return !isPaused; }
    void enable() { isPaused = false; }
    void disable() { isPaused = true; }

protected:
    GameWindow& window;

private:
    bool isPaused = false;
};

class RectRendSys : public IRenderSys {
public:
    RectRendSys(GameWindow& window);

public: // IRenderSys interface
    virtual void update(entt::registry& reg, SDL_Renderer& sdl_renderer) override;

private:
    Shader shader;
    Camera camera;
};
