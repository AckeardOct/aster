#include "scn_asteroids.h"

#include "../components/cmp_basic.h"
#include "../components/cmp_render.h"
#include "../systems/sys_basic.h"
#include "../systems/sys_render.h"

#include <SDL2/SDL_render.h>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

AsteroidsScene::AsteroidsScene(GameWindow& window)
    : IScene(window)
{
    initBasicSystems();
    initRenderSystems();
    initEntities();
}

AsteroidsScene::~AsteroidsScene() {}

void AsteroidsScene::input(float dt, const SDL_Event& event)
{
}

void AsteroidsScene::update(float dt)
{
    for (auto& sys : basicSystems) {
        if (sys->isEnabled()) {
            sys->update(reg, dt);
        }
    }
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
    glm::vec2 wsize = window.getSize();
    glm::vec2 wcenter = window.getCenter();

    auto entity = reg.create();
    glm::vec2 pos(wcenter);
    glm::vec2 size(wsize / 8.0f);
    reg.assign<PositionCmp>(entity, pos, size);

    glm::ivec4 color(0xff, 0x00, 0x00, 0x00);
    reg.assign<RectRendCmp>(entity, color, color);
}
