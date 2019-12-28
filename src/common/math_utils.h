#pragma once

#include <glm/gtc/quaternion.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>

using Vec2f = glm::vec2;
using Vec3f = glm::vec3;
using Mat4f = glm::mat4;
using Quat = glm::quat;

struct Color {
    uint8_t r = 0x0;
    uint8_t g = 0x0;
    uint8_t b = 0x0;
    uint8_t a = 0x0;

    Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
};

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
