#include "game_window.h"

#include <SDL2/SDL.h>

#include "../scenes/scn_asteroids.h"
#include "logger.h"

GameWindow::GameWindow(int argc, char** argv)
    : config(argc, argv)
{
    this->initSDL();

    scene = new AsteroidsScene(*this);
}

GameWindow::~GameWindow()
{
    delete scene;
    scene = nullptr;

    SDL_DestroyRenderer(sdl_renderer);
    sdl_renderer = nullptr;

    SDL_DestroyWindow(sdl_window);
    sdl_window = nullptr;

    SDL_Quit();
}

void GameWindow::runLoop()
{
    const float frameLength_ms = 1000.f / config.fps;
    float dt = 0;
    uint32_t time_ms = SDL_GetTicks();
    while (!quitRequested) {
        { // check fps
            uint32_t newTime_ms = SDL_GetTicks();
            dt = newTime_ms - time_ms;
            if (dt < frameLength_ms) {
                uint32_t diff = frameLength_ms - dt;
                if (diff > 5) {
                    SDL_Delay(diff / 2);
                }
                continue;
            }
            time_ms = newTime_ms;
        }

        processInput(dt);
        update(dt);
        render(dt);
    }
}

Vec2f GameWindow::getSize() const
{
    int w, h;
    SDL_GetWindowSize(sdl_window, &w, &h);
    return Vec2f(w, h);
}

Vec2f GameWindow::getCenter() const
{
    return getSize() / 2.f;
}

SDL_Renderer& GameWindow::getRenderer()
{
    if (sdl_renderer == nullptr) {
        LogCritical("sdl_renderer is null");
    }
    return *sdl_renderer;
}

void GameWindow::initSDL()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        LogCritical("SDL_Init(SDL_INIT_VIDEO) failed. error: %s", SDL_GetError());
    }

    sdl_window = SDL_CreateWindow(config.title.c_str(),
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        config.width,
        config.height,
        SDL_WINDOW_SHOWN);
    if (sdl_window == nullptr) {
        LogCritical("SDL_CreateWindow() failed. error: %s", SDL_GetError());
    }

    sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
    if (sdl_renderer == nullptr) {
        LogCritical("SDL_CreateRenderer() failed. error: %s", SDL_GetError());
    }

    LogMsg("initSDL() success!");
}

void GameWindow::processInput(float dt)
{
    SDL_Event sdl_event;
    while (SDL_PollEvent(&sdl_event)) {
        if (scene) {
            scene->input(dt, sdl_event);
        }

        switch (sdl_event.type) {
        case SDL_QUIT:
            quitRequested = true;
            break;
        }
    }
}

void GameWindow::update(float dt)
{
    if (scene) {
        scene->update(dt);
    }
}

void GameWindow::render(float dt)
{
    SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x00, 0x00, 0xff);
    SDL_RenderClear(sdl_renderer);

    if (scene) {
        scene->render(dt);
    }

    drawGrid();

    SDL_RenderPresent(sdl_renderer);
}

void GameWindow::drawGrid()
{
    const auto& grid = config.dbgDrawGrid;
    if (!grid.enable) {
        return;
    }

    const Vec2f sz = getSize();
    const Vec2f resolution(grid.resolutionX, grid.resolutionY);
    const Vec2f offset(grid.offsetX, grid.offsetY);
    const Color color(0x00, 0xff, 0x00, 0x00);

    SDL_SetRenderDrawColor(sdl_renderer, (u_char)color.r, (u_char)color.g, (u_char)color.b, (u_char)color.a);

    int currX = offset.x();
    while (currX < sz.x()) {
        SDL_RenderDrawLine(sdl_renderer, currX, 0, currX, sz.y());
        currX += resolution.x();
    }

    int currY = offset.y();
    while (currY < sz.y()) {
        SDL_RenderDrawLine(sdl_renderer, 0, currY, sz.x(), currY);
        currY += resolution.y();
    }
}
