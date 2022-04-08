#pragma once

namespace MathUtils {
	float normalize(float value, float min, float max);
	float lerpf(float a, float b, float t);
	double toRadians(double angle);
	double getPI();

	template<typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
	T min(T o, T o1) {
		return o > o1 ? o1 : o;
	}

	template<typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
	T max(T o, T o1) {
		return o > o1 ? o : o1;
	}
}