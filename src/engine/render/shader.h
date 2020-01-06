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
    void setMat4f(StringRef param, const Mat4f& mat);

private:
    int getUniformLocation(StringRef param) const; // TODO: CACHE

private:
    uint shaderProgram = 0;
    String vertexShaderFile;
    String fragmentShaderFile;
};
