#include "game_window.h"
#include <SDL2/SDL.h>

static const struct
{
  int width = 800;
  int height = 600;
  int fps = 30;
  const char* title = "Aster";
} SETTINGS;

GameWindow::GameWindow(int argc, char** argv)
{
  this->initSDL();
}

GameWindow::~GameWindow()
{
  SDL_DestroyRenderer(sdl_renderer);
  sdl_renderer = nullptr;

  SDL_DestroyWindow(sdl_window);
  sdl_window = nullptr;

  SDL_Quit();
}

void
GameWindow::runLoop()
{
  const float frameLength_ms = 1000.f / SETTINGS.fps;
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

glm::vec2
GameWindow::getSize() const
{
  int w, h;
  SDL_GetWindowSize(sdl_window, &w, &h);
  return glm::vec2(w, h);
}

void
GameWindow::initSDL()
{
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL_Init(SDL_INIT_VIDEO) failed. error: %s", SDL_GetError());
  }

  sdl_window = SDL_CreateWindow(SETTINGS.title,
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                SETTINGS.width,
                                SETTINGS.height,
                                SDL_WINDOW_SHOWN);
  if (sdl_window == nullptr) {
    printf("SDL_CreateWindow() failed. error: %s", SDL_GetError());
  }

  sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
  if (sdl_renderer == nullptr) {
    printf("SDL_CreateRenderer() failed. error: %s", SDL_GetError());
  }

  printf("initSDL() success!");
}

void
GameWindow::processInput(float dt)
{
  SDL_Event sdl_event;
  while (SDL_PollEvent(&sdl_event)) {

    switch (sdl_event.type) {
      case SDL_QUIT:
        quitRequested = true;
        break;
    }
  }
}

void
GameWindow::render(float dt)
{
  SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x00, 0x00, 0xff);
  SDL_RenderClear(sdl_renderer);

  SDL_RenderPresent(sdl_renderer);
}
