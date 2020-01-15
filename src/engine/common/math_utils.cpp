#include "math_utils.h"

#include "common/logger.h"

#include <math.h>

constexpr float MIN_DIFF_FLOAT = 0.00001;

bool isZero(const Vec2f& vec)
{
    if (std::abs(vec.x) > MIN_DIFF_FLOAT) {
        return false;
    } else if (std::abs(vec.y) > MIN_DIFF_FLOAT) {
        return false;
    }
    return true;
}

bool isZero(const Vec3f& vec)
{
    if (std::abs(vec.x) > MIN_DIFF_FLOAT) {
        return false;
    } else if (std::abs(vec.y) > MIN_DIFF_FLOAT) {
        return false;
    } else if (std::abs(vec.z) > MIN_DIFF_FLOAT) {
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
    , position(Vec3f(0))
{
}

void Transform::getModelMatrix(Mat4f& modelOut) const
{
    modelOut = glm::identity<Mat4f>();
    modelOut = glm::translate(modelOut, Vec3f(position.x, -position.y, position.z));
    modelOut = modelOut * glm::mat4_cast(orientation);
    modelOut = glm::scale(modelOut, sizeScale);
}
