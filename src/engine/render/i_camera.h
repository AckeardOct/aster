#pragma once

#include "common/math_utils.h"

class GameWindow;

class ICamera {
public:
    ICamera(GameWindow& window)
        : window(window)
    {
    }

    virtual void getViews(Mat4f& view, Mat4f& projection) const = 0;

protected:
    GameWindow& window;
};
