#include "math_utils.h"

#include "common/logger.h"

#include <math.h>

constexpr float MIN_DIFF_FLOAT = 0.00001;

Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
    : r(r)
    , g(g)
    , b(b)
    , a(a)
{
}

bool isZero(const Vec2f& vec)
{
    if (std::abs(vec.x) > MIN_DIFF_FLOAT) {
        return false;
    } else if (std::abs(vec.y) > MIN_DIFF_FLOAT) {
        return false;
    }
    return true;
}

Vec2f normalize(const Vec2f& vec)
{
    ASSERT(!isZero(vec));
    float length = vec.length();
    Vec2f ret = vec;
    ret.x /= length;
    ret.y /= length;
    return ret;
}

Transform::Transform()
    : sizeScale(Vec3f(1))
    , orientation(glm::angleAxis(0.f, Vec3f(0, 1, 0)))
{
}

Mat4f Transform::toMat4f() const
{
    const Mat4f scaleMatrix = scale(Mat4f(), sizeScale);
    const Mat4f rotationMatrix = glm::mat4_cast(orientation);
    const Mat4f translateMatrix = translate(Mat4f(), position);

    return translateMatrix * rotationMatrix * scaleMatrix;
}
