#include "sys_render.h"

#include "common/game_window.h"
#include "common/logger.h"
#include "common/math_utils.h"
#include "components/cmp_basic.h"
#include "components/cmp_render.h"

#include <GLES3/gl3.h>

#include "render/camera_2d.h"

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
    glm::mat4 viewM;
    glm::mat4 projectionM;
    camera.getViews(viewM, projectionM);
    shader.setMat4f("view", viewM);
    shader.setMat4f("projection", projectionM);

    auto view = reg.view<PositionCmp, RectRendCmp>();
    for (auto ent : view) {
        auto& posCmp = view.get<PositionCmp>(ent);
        auto& rectCmp = view.get<RectRendCmp>(ent);

        glm::mat4 modelM = glm::identity<glm::mat4>();
        posCmp.trans.getModelMatrix(modelM);
        shader.setMat4f("model", modelM);

        // TODO: make border color
        shader.setVec4f("color", rectCmp.fillColor);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
}
