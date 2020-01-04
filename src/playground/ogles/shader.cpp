#include "shader.h"

#include "common/file_system.h"
#include "common/logger.h"

#include <GLES3/gl3.h>

static GLuint loadShader(GLenum shaderType, const char* source)
{
    ASSERT(GL_VERTEX_SHADER == shaderType || GL_FRAGMENT_SHADER == shaderType);

    GLuint retShader = 0;
    retShader = glCreateShader(shaderType);
    if (0 == retShader) {
        ASSERT_FAIL("Can't create shader");
        return 0;
    }

    glShaderSource(retShader, 1, &source, NULL);
    glCompileShader(retShader);

    GLint compiledStatus = 0;
    glGetShaderiv(retShader, GL_COMPILE_STATUS, &compiledStatus);

    if (0 == compiledStatus) {
        GLint infoLen = 0;
        glGetShaderiv(retShader, GL_INFO_LOG_LENGTH, &infoLen);
        ASSERT(infoLen > 0);
        String info;
        info.resize(static_cast<size_t>(infoLen));
        glGetShaderInfoLog(retShader, infoLen, NULL, info.data());
        ASSERT_FAIL("Can't compile shader. error: %s", info.c_str());
        glDeleteShader(retShader);
        return 0;
    }
    return retShader;
}

Shader::Shader(StringRef vertexShaderFile, StringRef fragmentShaderFile)
    : vertexShaderFile(vertexShaderFile)
    , fragmentShaderFile(fragmentShaderFile)
{
}

Shader::~Shader()
{
    glDeleteProgram(shaderProgram);
}

bool Shader::compile()
{
    GLuint vertexShader = 0;
    String vertexSrc = fs::cat(vertexShaderFile);
    vertexShader = loadShader(GL_VERTEX_SHADER, vertexSrc.data());

    GLuint fragmentShader = 0;
    String fragmentSrc = fs::cat(fragmentShaderFile);
    fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentSrc.data());

    shaderProgram = glCreateProgram();
    if (0 == shaderProgram) {
        ASSERT_FAIL("Can't create shader program");
        return false;
    }
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    GLint linkStatus = 0;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkStatus);
    if (0 == linkStatus) {
        GLint infoLen = 0;
        glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &infoLen);
        ASSERT(infoLen > 0);
        String info;
        info.resize(static_cast<size_t>(infoLen));
        glGetProgramInfoLog(shaderProgram, infoLen, NULL, info.data());
        ASSERT_FAIL("Can't link shader program. error: %s", info.c_str());
        glDeleteProgram(shaderProgram);
        return false;
    }

    return true;
}

void Shader::setVec4f(StringRef param, const Vec4f& vec)
{
    GLint location = getUniformLocation(param);
    glUniform4f(location, vec.x, vec.y, vec.z, vec.w);
}

int Shader::getUniformLocation(StringRef param) const
{
    GLint location = glGetUniformLocation(shaderProgram, param.data());

    // check error
    switch (location) {
    case GL_INVALID_VALUE:
        ASSERT_FAIL("Can't get location of uniform: %s. error: GL_INVALID_VALUE.", param.data());
        break;
    case GL_INVALID_OPERATION:
        ASSERT_FAIL("Can't get location of uniform: %s. error: GL_INVALID_OPERATION.", param.data());
        break;
    }

    return location;
}
