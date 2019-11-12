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
    , physWorld(b2Vec2(0.f, 9.2f))
{
    initBasicSystems();
    initRenderSystems();
    initEntities();
}

AsteroidsScene::~AsteroidsScene()
{
    inputSystem = nullptr;
    for (auto& sys : basicSystems) {
        delete sys;
    }
    basicSystems.clear();

    for (auto& sys : renderSystems) {
        delete sys;
    }
    renderSystems.clear();
}

void AsteroidsScene::input(float dt, const SDL_Event& event)
{
    if (inputSystem) {
        inputSystem->onInput(event);
    }
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
    inputSystem = new InputSys();
    basicSystems.push_back(inputSystem);
    basicSystems.push_back(new PhysSys(physWorld));
}

void AsteroidsScene::initRenderSystems()
{
    renderSystems.push_back(new RectRendSys());
}

void AsteroidsScene::initEntities()
{
    const glm::vec2 wsize = window.getSize();
    const glm::vec2 wcenter = window.getCenter();

    { // player
        auto entity = reg.create();
        glm::vec2 pos(wcenter.x - 10, wcenter.y - 10);
        //glm::vec2 size(wsize / 8.0f);
        glm::vec2 size(glm::vec2(20, 20));
        reg.assign<PositionCmp>(entity, pos, size);

        glm::ivec4 color(0xff, 0x00, 0x00, 0x00);
        reg.assign<RectRendCmp>(entity, color, color);

        float speed = 100.f;
        reg.assign<MoveCmp>(entity, glm::vec2(speed, speed));
        reg.assign<InputableCmp>(entity);

        reg.assign<PhysBodyCmp>(entity, physWorld, PhysBodyCmp::Type::dynamicBody, pos, size);
    }

    if (false) { // plane
        auto entity = reg.create();
        glm::vec2 pos(200, 280);
        glm::vec2 size(100, 20);
        reg.assign<PositionCmp>(entity, pos, size);

        glm::ivec4 color(0x00, 0x00, 0xff, 0x00);
        reg.assign<RectRendCmp>(entity, color, color);

        reg.assign<PhysBodyCmp>(entity, physWorld, PhysBodyCmp::Type::staticBody, pos, size);
    }

    if (true) { // walls
        glm::vec2 size(0, wsize.y);
        { // left
            auto entity = reg.create();
            glm::vec2 pos(-1, wcenter.y);
            reg.assign<PositionCmp>(entity, pos, size);

            //glm::ivec4 color(0x00, 0x00, 0xff, 0x00);
            //reg.assign<RectRendCmp>(entity, color, color);

            reg.assign<PhysBodyCmp>(entity, physWorld, PhysBodyCmp::Type::staticBody, pos, size);
        }
        { // right
            auto entity = reg.create();
            glm::vec2 pos(wsize.x + 1, wcenter.y);
            reg.assign<PositionCmp>(entity, pos, size);

            //glm::ivec4 color(0x00, 0x00, 0xff, 0x00);
            //reg.assign<RectRendCmp>(entity, color, color);

            reg.assign<PhysBodyCmp>(entity, physWorld, PhysBodyCmp::Type::staticBody, pos, size);
        }
        size = glm::vec2(wsize.x, 0);
        { // top
            auto entity = reg.create();
            glm::vec2 pos(wcenter.x, -1);
            reg.assign<PositionCmp>(entity, pos, size);

            //glm::ivec4 color(0x00, 0x00, 0xff, 0x00);
            //reg.assign<RectRendCmp>(entity, color, color);

            reg.assign<PhysBodyCmp>(entity, physWorld, PhysBodyCmp::Type::staticBody, pos, size);
        }
        { // bottom
            auto entity = reg.create();
            glm::vec2 pos(wcenter.x, wsize.y + 1);
            reg.assign<PositionCmp>(entity, pos, size);

            //glm::ivec4 color(0x00, 0x00, 0xff, 0x00);
            //reg.assign<RectRendCmp>(entity, color, color);

            reg.assign<PhysBodyCmp>(entity, physWorld, PhysBodyCmp::Type::staticBody, pos, size);
        }
    }
}
