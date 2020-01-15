#pragma once

#include "common/math_utils.h"

class ICamera {
public:
    virtual void getViews(Mat4f& view, Mat4f& projection) const = 0;
};
