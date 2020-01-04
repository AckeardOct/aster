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

// Преобразует координаты из локальных в мировые в следующем порядке:
//  - сначала вершины масштабируются
//    например, единичный цилиндр превращается в диск или в трубку
//  - затем поворачиваются
//    т.е. тела ориентируются в пространстве
//  - затем переносятся
//    т.е. задаётся положение тела
// изменив порядок, мы изменили бы значение трансформаций.
class Transform {
public:
    // Конструирует трансформацию с
    //  - единичным масштабированием;
    //  - нулевым вращением вокруг оси Oy;
    //  - нулевой позицией.
    Transform();

    // Преобразует исходную трансформацию в матрицу 4x4.
    Mat4f toMat4f() const;

    Vec3f sizeScale;
    Quat orientation;
    Vec3f position;
};

bool isZero(const Vec2f& vec);
Vec2f normalize(const Vec2f& vec);
