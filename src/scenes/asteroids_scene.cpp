#include "asteroids_scene.h"

#include <SDL2/SDL_render.h>

AsteroidsScene::AsteroidsScene(GameWindow& window)
    : IScene(window)
{
}

AsteroidsScene::~AsteroidsScene() {}

void AsteroidsScene::input(float dt, const SDL_Event& event)
{
}

void AsteroidsScene::update(float dt)
{
}

void AsteroidsScene::render(float dt)
{
    SDL_Renderer& renderer = window.getRenderer();

    glm::vec2 vec = window.getSize();
    int SCREEN_WIDTH = vec.x;
    int SCREEN_HEIGHT = vec.y;

    SDL_Rect fillRect = {
        SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2
    };
    SDL_SetRenderDrawColor(&renderer, 0xFF, 0x00, 0x00, 0xFF);
    SDL_RenderFillRect(&renderer, &fillRect);
}
