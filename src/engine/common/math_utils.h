#pragma once

#include <glm/gtc/quaternion.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

using Vec2f = glm::vec2;
using Vec3f = glm::vec3;
using Vec4f = glm::vec4;
using Mat4f = glm::mat4;
using Quat = glm::quat;
using Color = Vec4f;

// GLOBALS
const Vec2f Vec2f_Zero(0.f, 0.f);

const Vec3f Vec3f_Zero(0.f, 0.f, 0.f);
const Vec3f Vec3f_UnitX(1.f, 0.f, 0.f);
const Vec3f Vec3f_UnitY(0.f, 1.f, 0.f);
const Vec3f Vec3f_UnitZ(0.f, 0.f, 1.f);

class Transform {
public:
    Transform();

    void getModelMatrix(Mat4f& model) const;

    Vec3f sizeScale;
    Quat orientation;
    Vec3f position;
};

bool isZero(const Vec2f& vec);
bool isZero(const Vec3f& vec);
Vec2f normalize(const Vec2f& vec);
