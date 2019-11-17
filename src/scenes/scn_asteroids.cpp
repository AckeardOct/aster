#include "scn_asteroids.h"

#include "components/cmp_basic.h"
#include "components/cmp_render.h"
#include "systems/sys_basic.h"
#include "systems/sys_render.h"

#include <SDL2/SDL_render.h>

AsteroidsScene::AsteroidsScene(GameWindow& window)
    : IScene(window)
    , physWorld(b2Vec2(0.f, 1.f))
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
    physWorld.Step(dt, 6, 2);
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
    const Vec2f wsize = window.getSize();
    const Vec2f wcenter = window.getCenter();

    { // player
        auto entity = reg.create();
        Vec2f pos(wcenter.x(), wcenter.y());
        Vec2f size(20.f, 20.f);
        reg.assign<PositionCmp>(entity, pos, size);

        Color color(0xff, 0x00, 0x00, 0x00);
        reg.assign<RectRendCmp>(entity, color, color);

        float speed = 100.f;
        reg.assign<MoveCmp>(entity, Vec2f(speed, speed));
        reg.assign<InputableCmp>(entity);

        reg.assign<PhysDynamicBodyCmp>(entity, physWorld, pos, size);
    }

    { // cube
        auto entity = reg.create();
        Vec2f pos(wcenter.x() - 40, wcenter.y());
        Vec2f size(Vec2f(20, 20));
        reg.assign<PositionCmp>(entity, pos, size);

        Color color(0x00, 0xff, 0x00, 0x00);
        reg.assign<RectRendCmp>(entity, color, color);
        reg.assign<PhysDynamicBodyCmp>(entity, physWorld, pos, size);
    }

    { // platform
        auto entity = reg.create();
        Vec2f size(wcenter.x() / 2, 10);
        Vec2f pos(wcenter.x(), wsize.y() * 0.75);
        reg.assign<PositionCmp>(entity, pos, size);

        Color color(0x00, 0x00, 0xff, 0x00);
        reg.assign<RectRendCmp>(entity, color, color);
        reg.assign<PhysStaticBodyCmp>(entity, physWorld, pos, size);
    }

    { // walls
        Vec2f size(0, wsize.y());
        { // left
            auto entity = reg.create();
            Vec2f pos(-1, wcenter.y());
            reg.assign<PositionCmp>(entity, pos, size);
            reg.assign<PhysStaticBodyCmp>(entity, physWorld, pos, size);
        }
        { // right
            auto entity = reg.create();
            Vec2f pos(wsize.x() + 1, wcenter.y());
            reg.assign<PositionCmp>(entity, pos, size);
            reg.assign<PhysStaticBodyCmp>(entity, physWorld, pos, size);
        }
        size = Vec2f(wsize.x(), 0);
        { // top
            auto entity = reg.create();
            Vec2f pos(wcenter.x(), -1);
            reg.assign<PositionCmp>(entity, pos, size);
            reg.assign<PhysStaticBodyCmp>(entity, physWorld, pos, size);
        }
        { // bottom
            auto entity = reg.create();
            Vec2f pos(wcenter.x(), wsize.y() + 1);
            reg.assign<PositionCmp>(entity, pos, size);
            reg.assign<PhysStaticBodyCmp>(entity, physWorld, pos, size);
        }
    }
}
