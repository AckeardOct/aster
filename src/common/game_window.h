#pragma once

#include "common/game_config.h"
#include "common/math_utils.h"

class GameConfig;
class IScene;
class SDL_Renderer;
class SDL_Window;

class GameWindow {
public:
    GameWindow(int argc, char** argv);
    ~GameWindow();

    void runLoop();
    Vec2f getSize() const;
    Vec2f getCenter() const;
    SDL_Renderer& getRenderer();

private:
    void initSDL(void);
    void processInput(float dt);
    void update(float dt);
    void render(float dt);

    // dbg draw
    void drawGrid();

private:
    GameConfig config;
    SDL_Window* sdl_window = nullptr;
    SDL_Renderer* sdl_renderer = nullptr;
    IScene* scene = nullptr;

    bool quitRequested = false;
};
