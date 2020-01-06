#include "sys_render.h"

#include "common/game_window.h"
#include "common/logger.h"
#include "common/math_utils.h"
#include "components/cmp_basic.h"
#include "components/cmp_render.h"

#include <GLES3/gl3.h>
#include <SDL2/SDL_render.h>

IRenderSys::IRenderSys(GameWindow& window)
    : window(window)
{
}

static GLuint VAO = 0;
static GLuint VBO = 0;
static GLuint EBO = 0;

RectRendSys::RectRendSys(GameWindow& window)
    : IRenderSys(window)
    , shader("./shaders/simple.vsh", "./shaders/simple.fsh")
    , camera(Vec3f(0, 0, 0))
{
    shader.compile();

    GLfloat vertices[] = {
        +0.5f, +0.5f, +0.f,
        +0.5f, -0.5f, +0.f,
        -0.5f, -0.5f, +0.f,
        -0.5f, +0.5f, +0.f
    };
    GLuint indeces[] = {
        0, 1, 3,
        1, 2, 3
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indeces), indeces, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

#ifdef USE_GL_RENDER
void RectRendSys::update(entt::registry& reg, SDL_Renderer& sdl_renderer)
{
    const Vec2f winSize = window.getSize();
    const Vec2f winCenter = window.getCenter();

    const float magic = 1.2085f;
    camera.Position = Vec3f(winCenter.x, -winCenter.y, winSize.x * magic);

    glUseProgram(shader.handle());
    glm::mat4 viewM = camera.GetViewMatrix();
    shader.setMat4f("view", viewM);
    float ratio = winSize.x / winSize.y;
    glm::mat4 projectionM = glm::perspective(glm::radians(camera.Zoom), ratio, 0.1f, 1'000.0f);
    shader.setMat4f("projection", projectionM);

    auto view = reg.view<PositionCmp, RectRendCmp>();
    for (auto ent : view) {
        auto& posCmp = view.get<PositionCmp>(ent);
        auto& rectCmp = view.get<RectRendCmp>(ent);

        glm::mat4 modelM = glm::identity<glm::mat4>();
        modelM = glm::translate(modelM, Vec3f(posCmp.pos.x, -posCmp.pos.y, 0.0f));
        // rotate
        modelM = glm::scale(modelM, Vec3f(posCmp.size.x, posCmp.size.y, 1.0f));
        shader.setMat4f("model", modelM);

        // TODO: make border color
        shader.setVec4f("color", rectCmp.fillColor);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
}

#else
void RectRendSys::update(entt::registry& reg, SDL_Renderer& sdl_renderer)
{
    SDL_Rect rect;
    auto view = reg.view<PositionCmp, RectRendCmp>();
    for (auto ent : view) {
        auto& posCmp = view.get<PositionCmp>(ent);
        auto& rectCmp = view.get<RectRendCmp>(ent);

        rect.w = static_cast<int>(posCmp.size.x);
        rect.h = static_cast<int>(posCmp.size.y);
        rect.x = static_cast<int>(posCmp.pos.x - rect.w / 2);
        rect.y = static_cast<int>(posCmp.pos.y - rect.h / 2);

        { // fill
            const Color& color = rectCmp.fillColor;
            SDL_SetRenderDrawColor(&sdl_renderer, (u_char)color.r, (u_char)color.g, (u_char)color.b, (u_char)color.a);
            SDL_RenderFillRect(&sdl_renderer, &rect);
        }

        { // border
            const Color& color = rectCmp.borderColor;
            SDL_SetRenderDrawColor(&sdl_renderer, (u_char)color.r, (u_char)color.g, (u_char)color.b, (u_char)color.a);
            SDL_RenderDrawRect(&sdl_renderer, &rect);
        }
    }
}
#endif
