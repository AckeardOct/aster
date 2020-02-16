#include "sys_render.h"

#include "common/game_window.h"
#include "common/logger.h"
#include "common/math_utils.h"
#include "components/cmp_basic.h"
#include "components/cmp_render.h"

#include <GLES3/gl3.h>

IRenderSys::IRenderSys(GameWindow& window)
    : window(window)
{
}

RectRendSys::RectRendSys(GameWindow& window)
    : IRenderSys(window)
    , shader("./shaders/simple.vsh", "./shaders/simple.fsh")
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

void RectRendSys::update(entt::registry& reg, const ICamera& camera)
{
    glUseProgram(shader.handle());
    Mat4f viewM;
    Mat4f projectionM;
    camera.getViews(viewM, projectionM);
    shader.setMat4f("view", viewM);
    shader.setMat4f("projection", projectionM);

    auto view = reg.view<PositionCmp, RectRendCmp>();
    for (auto ent : view) {
        auto& posCmp = view.get<PositionCmp>(ent);
        auto& rectCmp = view.get<RectRendCmp>(ent);

        Mat4f modelM = glm::identity<Mat4f>();
        posCmp.trans.getModelMatrix(modelM);
        shader.setMat4f("model", modelM);

        // TODO: make border color
        shader.setVec4f("color", rectCmp.fillColor);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
}

CubeRendSys::CubeRendSys(GameWindow& window)
    : IRenderSys(window)
    , shader("./shaders/simple.vsh", "./shaders/simple.fsh")
{
    shader.compile();

    // 3 float - vertix
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,
        +0.5f, -0.5f, -0.5f,
        +0.5f, -0.5f, -0.5f,
        +0.5f, -0.5f, -0.5f,
        -0.5f, +0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f, +0.5f,
        +0.5f, -0.5f, +0.5f,
        +0.5f, +0.5f, +0.5f,
        +0.5f, +0.5f, +0.5f,
        -0.5f, +0.5f, +0.5f,
        -0.5f, -0.5f, +0.5f,

        -0.5f, +0.5f, +0.5f,
        -0.5f, +0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, +0.5f,
        -0.5f, +0.5f, +0.5f,

        +0.5f, +0.5f, +0.5f,
        +0.5f, +0.5f, -0.5f,
        +0.5f, -0.5f, -0.5f,
        +0.5f, -0.5f, -0.5f,
        +0.5f, -0.5f, +0.5f,
        +0.5f, +0.5f, +0.5f,

        -0.5f, -0.5f, -0.5f,
        +0.5f, -0.5f, -0.5f,
        +0.5f, -0.5f, +0.5f,
        +0.5f, -0.5f, +0.5f,
        -0.5f, -0.5f, +0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, +0.5f, -0.5f,
        +0.5f, +0.5f, -0.5f,
        +0.5f, +0.5f, +0.5f,
        +0.5f, +0.5f, +0.5f,
        -0.5f, +0.5f, +0.5f,
        -0.5f, +0.5f, -0.5f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void CubeRendSys::update(entt::registry& reg, const ICamera& camera)
{
    const Vec2f wsize = window.getSize();
    glUseProgram(shader.handle());

    Mat4f view;
    Mat4f projection;
    camera.getViews(view, projection);
    shader.setMat4f("projection", projection);
    shader.setMat4f("view", view);

    auto cubesView = reg.view<PositionCmp, CubeRendCmp>();
    for (auto ent : cubesView) {
        auto& posCmp = cubesView.get<PositionCmp>(ent);
        auto& rectCmp = cubesView.get<CubeRendCmp>(ent);

        Mat4f model = glm::identity<Mat4f>();
        posCmp.trans.getModelMatrix(model);
        shader.setMat4f("model", model);

        // TODO: make border color
        shader.setVec4f("color", rectCmp.fillColor);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}
