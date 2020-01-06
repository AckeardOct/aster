#include "game_window.h"

#include <GLES3/gl3.h>
#include <SDL2/SDL.h>

#include "common/logger.h"
#include "scenes/scn_asteroids.h"

GameWindow::GameWindow(int argc, char** argv)
    : config(argc, argv)
{
    initSDL();
    initOpenGl();
    scene = new AsteroidsScene(*this);
}

GameWindow::~GameWindow()
{
    SDL_GL_DeleteContext(gl_context);
    gl_context = nullptr;

    delete scene;
    scene = nullptr;

    SDL_DestroyRenderer(sdl_renderer);
    sdl_renderer = nullptr;

    SDL_DestroyWindow(sdl_window);
    sdl_window = nullptr;

    SDL_Quit();
}

bool GameWindow::initSDL()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        ASSERT_FAIL("SDL_Init(SDL_INIT_VIDEO) failed. error: %s", SDL_GetError());
        return false;
    }

    sdl_window = SDL_CreateWindow(config.window.title.c_str(),
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        config.window.width,
        config.window.height,
        SDL_WINDOW_SHOWN);
    if (sdl_window == nullptr) {
        ASSERT_FAIL("SDL_CreateWindow() failed. error: %s", SDL_GetError());
        return false;
    }

    sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
    if (sdl_renderer == nullptr) {
        ASSERT_FAIL("SDL_CreateRenderer() failed. error: %s", SDL_GetError());
        return false;
    }

    return true;
}

bool GameWindow::initOpenGl()
{
    gl_context = SDL_GL_CreateContext(sdl_window);
    if (gl_context == nullptr) {
        ASSERT_FAIL("Can't create OpenGlContext");
        return false;
    }

    int res = 0;
    res = SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    ASSERT(0 == res);

    res = SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    ASSERT(0 == res);

    res = SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    ASSERT(0 == res);

    res = SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    ASSERT(0 == res);

    res = SDL_GL_SetSwapInterval(1);
    ASSERT(0 == res);

    glEnable(GL_DEPTH_TEST);

    return true;
}

void GameWindow::runLoop()
{
    const float frameLength_ms = 1000.f / config.window.fps;
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
        ASSERT_FAIL("sdl_renderer is null");
    }
    return *sdl_renderer;
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
#ifdef USE_GL_RENDER
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
#else
    glClearColor(0.f, 0.f, 0.f, 1.0f);
#endif
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (scene) {
        scene->render(dt);
    }

#ifdef USE_GL_RENDER
    SDL_GL_SwapWindow(sdl_window);
#else
    SDL_RenderPresent(sdl_renderer);
#endif
}
