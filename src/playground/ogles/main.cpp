#include <GLES3/gl3.h>
#include <SDL2/SDL.h>

#include "shader.h"

#include "common/file_system.h"
#include "common/logger.h"
#include "common/math_utils.h"
#include "common/string.h"

const String SHADERS_DIR = "./shaders/";
const int winWidth = 640;
const int winHeight = 480;

SDL_Window* sdl_window = nullptr;
SDL_Renderer* sdl_renderer = nullptr;

SDL_GLContext gl_context = nullptr;
Shader shader(SHADERS_DIR + "/simple.vsh", SHADERS_DIR + "/simple.fsh");

void initSDL()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        ASSERT_FAIL("SDL_Init(SDL_INIT_VIDEO) failed. error: %s", SDL_GetError());
    }

    sdl_window = SDL_CreateWindow("OGLES",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        winWidth, winHeight,
        SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    if (sdl_window == nullptr) {
        ASSERT_FAIL("SDL_CreateWindow() failed. error: %s", SDL_GetError());
    }

    sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
    if (sdl_renderer == nullptr) {
        ASSERT_FAIL("SDL_CreateRenderer() failed. error: %s", SDL_GetError());
    }

    LogMsg("initSDL() success!");
}

void initOpenGlEs()
{
    gl_context = SDL_GL_CreateContext(sdl_window);
    ASSERT(gl_context);

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

    LogMsg("initOpenGlEs() success!");
}

void destroySDL()
{
    SDL_GL_DeleteContext(gl_context);

    SDL_DestroyRenderer(sdl_renderer);
    sdl_renderer = nullptr;

    SDL_DestroyWindow(sdl_window);
    sdl_window = nullptr;

    SDL_Quit();
}

GLuint VAO = 0;
GLuint VBO = 0;
GLuint EBO = 0;

void initRenderObjects()
{
    shader.compile();

    GLfloat vertices[] = {
        +0.5, +0.5, 0.0,
        +0.5, -0.5, 0.0,
        -0.5, -0.5, 0.0,
        -0.5, 0.5, 0.0
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(GL_NONE);

    // clear
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
    glBindVertexArray(GL_NONE);
}

void destroyRenderObjects()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void render()
{
    glViewport(0, 0, winWidth, winHeight);
    glClearColor(0.0, 1.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    { // tringle
        glUseProgram(shader.handle());
        Vec4f blue(0.0, 0.0, 1.0, 1.0);
        shader.setVec4f("ourColor", blue); // blue

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindVertexArray(GL_NONE);
    }

    SDL_GL_SwapWindow(sdl_window);
}

int main(int argc, char** argv)
{
    initSDL();
    initOpenGlEs();
    initRenderObjects();

    bool quitRequested = false;
    while (!quitRequested) {
        render();

        SDL_Event sdl_event;
        while (SDL_PollEvent(&sdl_event)) {
            switch (sdl_event.type) {
            case SDL_QUIT:
                quitRequested = true;
                break;
            }
        }
    }

    destroyRenderObjects();
    destroySDL();
    LogMsg("GoodBye!");
    return 0;
}
