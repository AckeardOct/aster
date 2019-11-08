#pragma once

#include "../common/game_window.h"
#include <SDL2/SDL_events.h>

class GameWindow;

class IScene
{
public:
  IScene(GameWindow& window)
    : window(window)
  {}
  virtual ~IScene() {}

public: // abstract interface
  virtual void input(float dt, const SDL_Event& event) = 0;
  virtual void update(float dt) = 0;
  virtual void render(float dt) = 0;

protected:
  GameWindow& window;
};
