#pragma once

#include "common/math_utils.h"
#include "common/string.h"

class Shader {
public:
    explicit Shader(StringRef vertexShaderFile, StringRef fragmentShaderFile);
    ~Shader();

    bool compile();

    uint handle() const { return shaderProgram; }
    void setVec4f(StringRef param, const Vec4f& vec);

private:
    int getUniformLocation(StringRef param) const;

private:
    uint shaderProgram = 0;
    String vertexShaderFile;
    String fragmentShaderFile;
};
