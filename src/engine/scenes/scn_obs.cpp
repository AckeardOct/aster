#include "scn_obs.h"

#include "components/cmp_basic.h"
#include "components/cmp_render.h"
#include "render/camera_3d.h"
#include "systems/sys_basic.h"
#include "systems/sys_render.h"

ObsScene::ObsScene(GameWindow& window)
    : IScene(window)
{
    camera = new Camera3D(window, Vec3f(0.f, 0.f, 3.f));

    initBasicSystems();
    initRenderSystems();
    initEntities();
}

ObsScene::~ObsScene()
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

void ObsScene::input(float dt, const SDL_Event& event)
{
    if (inputSystem) {
        inputSystem->onInput(event);
    }
}

void ObsScene::update(float dt)
{
    for (auto& sys : basicSystems) {
        if (sys->isEnabled()) {
            sys->update(reg, dt);
        }
    }
}

void ObsScene::render(float dt)
{
    for (auto& sys : renderSystems) {
        if (sys->isEnabled()) {
            sys->update(reg, *camera);
        }
    }
}

void ObsScene::initBasicSystems()
{
    inputSystem = new InputSys();
    basicSystems.push_back(inputSystem);
}

void ObsScene::initRenderSystems()
{
    //renderSystems.push_back(new RectRendSys(window));
    renderSystems.push_back(new CubeRendSys(window));
}

void ObsScene::initEntities()
{
    const Vec2f wsize = window.getSize();
    const Vec2f wcenter = window.getCenter();

    { // cube
        auto entity = reg.create();
        Vec2f size(2.f, 1.f);
        Vec2f pos(0.f, 0.f);
        reg.assign<PositionCmp>(entity, pos, size);

        Color color(0x00, 0x00, 0xff, 0x00);
        reg.assign<CubeRendCmp>(entity, color, color);
    }
}
