#pragma once

#include "i_camera.h"

class Camera2D : public ICamera {
public:
    Camera2D(const Vec2f& area, const Vec2f& offset = Vec2f_Zero);

public: // ICamera interface
    void getViews(Mat4f& view, Mat4f& projection) const;

private:
    Vec2f area;
    Vec2f offset;
};
