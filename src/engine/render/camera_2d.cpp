#include "camera_2d.h"

Camera2D::Camera2D(const Vec2f& area, const Vec2f& offset)
    : area(area)
    , offset(offset)
{
}

void Camera2D::getViews(Mat4f& view, Mat4f& projection) const
{
    view = glm::identity<Mat4f>();
    view = glm::scale(view, Vec3f(2.f * 1.f / area.x, 2.f * 1.f / area.y, 1.f));
    view = glm::translate(view, Vec3f(-area.x / 2.f + offset.x, area.y / 2.f + offset.y, 0.f));

    projection = glm::identity<Mat4f>();
}
