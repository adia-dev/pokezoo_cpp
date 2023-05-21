#include "math_utils.h"

float MathUtils::lerp(float a, float b, float f) { return a + f * (b - a); }