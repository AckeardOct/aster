#pragma once

#include "i_scene.h"

class AsteroidsScene : public IScene
{
public:
  AsteroidsScene(GameWindow& window);
  virtual ~AsteroidsScene() override;

  // IScene interface
public:
  virtual void input(float dt, const SDL_Event& event) override;
  virtual void update(float dt) override;
  virtual void render(float dt) override;
};
