#include <GLES3/gl3.h>
#include <SDL2/SDL.h>

#include "shader.h"

#include "common/file_system.h"
#include "common/logger.h"
#include "common/math_utils.h"
#include "common/string.h"

#include "stb-image/stb_image.h" // TODO: change library

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const String SHADERS_DIR = "./shaders/";
const String TEXTURES_DIR = "./textures";

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
GLuint texture = 0;

void initRenderObjects()
{
    shader.compile();

    float vertices[] = {
        // positions          // colors           // texture coords
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f // top left
    };

    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3 // second triangle
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    { // load and create texture
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // load image, create texture and generate mipmaps
        int width, height, nrChannels;
        String texturePath = TEXTURES_DIR + "/container.jpg";
        unsigned char* data = stbi_load(texturePath.data(), &width, &height, &nrChannels, 0);
        ASSERT_MSG(data, "Can't read texture: %s", texturePath.data());
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    }

    glEnableVertexAttribArray(GL_NONE);

    // clear
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
    glBindVertexArray(GL_NONE);
}

void destroyRenderObjects()
{
    glDeleteTextures(1, &texture);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void render()
{
    glViewport(0, 0, winWidth, winHeight);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    { // textured rectangle
        glUseProgram(shader.handle());

        glm::mat4 model = glm::identity<glm::mat4>();
        model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0, 0.0, 0.0));
        shader.setMat4f("model", model);

        glm::mat4 view = glm::identity<glm::mat4>();
        view = glm::translate(view, glm::vec3(0.0, 0.0, -3.0));
        shader.setMat4f("view", view);

        glm::mat4 projection = glm::identity<glm::mat4>();
        projection = glm::perspective(glm::radians(45.0f), (float)winWidth / winHeight, 0.1f, 100.0f);
        shader.setMat4f("projection", projection);

        glBindVertexArray(VAO);
        glBindTexture(GL_TEXTURE_2D, texture);
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
