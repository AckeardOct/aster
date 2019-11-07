#pragma once

#include <glm/vec2.hpp>

class SDL_Renderer;
class SDL_Window;

class GameWindow
{
public:
  GameWindow(int argc, char** argv);
  ~GameWindow();

  void runLoop();
  glm::vec2 getSize() const;

private:
  void initSDL(void);
  void processInput(float dt);
  void update(float dt) {}
  void render(float dt);

private:
  SDL_Window* sdl_window = nullptr;
  SDL_Renderer* sdl_renderer = nullptr;

  bool quitRequested = false;
};
