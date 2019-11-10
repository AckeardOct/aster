#pragma once

#include "i_scene.h"

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
    InputSys* inputSystem = nullptr;

    entt::registry reg;
    std::vector<IBasicSys*> basicSystems;
    std::vector<IRenderSys*> renderSystems;
};
