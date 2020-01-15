#pragma once

#include "i_scene.h"
#include "render/i_camera.h"

#include <Box2D/Dynamics/b2World.h>
#include <entt/entity/registry.hpp>
#include <vector>

class IBasicSys;
class IRenderSys;
class InputSys;

class AsteroidsScene : public IScene {
public:
    AsteroidsScene(GameWindow& window);
    virtual ~AsteroidsScene() override;

    // IScene interface
public:
    virtual void input(float dt, const SDL_Event& event) override;
    virtual void update(float dt) override;
    virtual void render(float dt) override;

private:
    void initBasicSystems();
    void initRenderSystems();
    void initEntities();

private:
    b2World physWorld;
    InputSys* inputSystem = nullptr;
    ICamera* camera = nullptr;

    entt::registry reg;
    std::vector<IBasicSys*> basicSystems;
    std::vector<IRenderSys*> renderSystems;
};
