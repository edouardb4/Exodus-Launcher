#include "MathUtils.h"

#include <type_traits>

static const double PI = 3.14159;

double MathUtils::toRadians(double angle) {
	return (angle / 180.0) * PI;
}

double MathUtils::getPI() {
	return PI;
}

float MathUtils::lerpf(float a, float b, float t) {
	return (1.0f - t) * a + b * t;
}

float MathUtils::normalize(float value, float min, float max) {
	return (value - min) / (max - min);
}