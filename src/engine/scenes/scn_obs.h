#pragma once

#include "i_scene.h"

#include <entt/entity/registry.hpp>

class IBasicSys;
class IRenderSys;
class InputSys;
class ICamera;

class ObsScene : public IScene {
public:
    ObsScene(GameWindow& window);
    virtual ~ObsScene() override;

public: // IScene interface
    virtual void input(float dt, const SDL_Event& event) override;
    virtual void update(float dt) override;
    virtual void render(float dt) override;

private:
    void initBasicSystems();
    void initRenderSystems();
    void initEntities();

private:
    InputSys* inputSystem = nullptr;
    ICamera* camera = nullptr;

    entt::registry reg;
    std::vector<IBasicSys*> basicSystems;
    std::vector<IRenderSys*> renderSystems;
};
