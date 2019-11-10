#include "asteroids_scene.h"

//#include "../components/basic.h"
//#include "../components/render.h"

#include <SDL2/SDL_render.h>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

class RectRendCmp {
public:
    RectRendCmp(glm::ivec4 borderColor, glm::ivec4 fillColor = glm::ivec4 { 0 })
        : borderColor(borderColor)
        , fillColor(fillColor)
    {
    }

public:
    glm::ivec4 borderColor;
    glm::ivec4 fillColor;
};

struct PositionCmp {
    glm::vec2 pos;
    glm::vec2 size;
    glm::vec2 direction;

public:
    PositionCmp(glm::vec2 pos, glm::vec2 size, glm::vec2 direction = glm::vec2(0.f, 0.f))
        : pos(pos)
        , size(size)
        , direction(direction)
    {
    }
};

class IRenderSys {
public:
    virtual void update(entt::registry& reg, SDL_Renderer& sdl_renderer) = 0;
    bool isEnabled() { return !isPaused; }
    void enable() { isPaused = false; }
    void disable() { isPaused = true; }

private:
    bool isPaused = false;
};

class RectRendSys : public IRenderSys {
public: // IRenderSys interface
    virtual void update(entt::registry& reg, SDL_Renderer& sdl_renderer) override
    {
        SDL_Rect rect;
        auto view = reg.view<PositionCmp, RectRendCmp>();
        for (auto ent : view) {
            auto& posCmp = view.get<PositionCmp>(ent);
            auto& rectCmp = view.get<RectRendCmp>(ent);

            rect.w = (int)posCmp.size.x;
            rect.h = (int)posCmp.size.y;
            rect.x = (int)posCmp.pos.x - rect.w / 2;
            rect.y = (int)posCmp.pos.y - rect.h / 2;

            { // fill
                const glm::ivec4& color = rectCmp.fillColor;
                SDL_SetRenderDrawColor(&sdl_renderer, (u_char)color.r, (u_char)color.g, (u_char)color.b, (u_char)color.a);
                SDL_RenderFillRect(&sdl_renderer, &rect);
            }

            { // border
                const glm::ivec4& color = rectCmp.borderColor;
                SDL_SetRenderDrawColor(&sdl_renderer, (u_char)color.r, (u_char)color.g, (u_char)color.b, (u_char)color.a);
                SDL_RenderDrawRect(&sdl_renderer, &rect);
            }
        }
    }
};

AsteroidsScene::AsteroidsScene(GameWindow& window)
    : IScene(window)
{
    initRenderSystems();
    initEntities();
}

AsteroidsScene::~AsteroidsScene() {}

void AsteroidsScene::input(float dt, const SDL_Event& event)
{
}

void AsteroidsScene::update(float dt)
{
}

void AsteroidsScene::render(float dt)
{
    SDL_Renderer& sdl_renderer = window.getRenderer();
    for (auto& sys : renderSystems) {
        if (sys->isEnabled()) {
            sys->update(reg, sdl_renderer);
        }
    }
}

void AsteroidsScene::initBasicSystems()
{
}

void AsteroidsScene::initRenderSystems()
{
    renderSystems.push_back(new RectRendSys());
}

void AsteroidsScene::initEntities()
{
    auto entity = reg.create();
    glm::vec2 pos(100, 100);
    glm::vec2 size(50, 50);
    reg.assign<PositionCmp>(entity, pos, size);

    glm::ivec4 color(0xff, 0x00, 0x00, 0x00);
    reg.assign<RectRendCmp>(entity, color, color);
}
