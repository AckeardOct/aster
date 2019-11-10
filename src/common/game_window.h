#pragma once

#include "game_config.h"
#include <glm/vec2.hpp>

class GameConfig;
class IScene;
class SDL_Renderer;
class SDL_Window;

class GameWindow {
public:
    GameWindow(int argc, char** argv);
    ~GameWindow();

    void runLoop();
    glm::vec2 getSize() const;
    glm::vec2 getCenter() const;
    SDL_Renderer& getRenderer();

private:
    void initSDL(void);
    void processInput(float dt);
    void update(float dt);
    void render(float dt);

private:
    GameConfig config;
    SDL_Window* sdl_window = nullptr;
    SDL_Renderer* sdl_renderer = nullptr;
    IScene* scene = nullptr;

    bool quitRequested = false;
};
