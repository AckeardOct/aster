#pragma once

#include "../common/math_utils.h"

class RectRendCmp {
public:
    RectRendCmp(Color borderColor, Color fillColor = Color(0, 0, 0, 0));

public:
    Color borderColor;
    Color fillColor;
};
