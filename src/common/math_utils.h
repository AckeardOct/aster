#pragma once

#include <Eigen/Geometry>

using Vec2f = Eigen::Vector2f;

struct Color {
    uint8_t r = 0x0;
    uint8_t g = 0x0;
    uint8_t b = 0x0;
    uint8_t a = 0x0;

    Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
};
