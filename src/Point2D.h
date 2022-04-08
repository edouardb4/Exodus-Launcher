#pragma once

#include <type_traits>

template<typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
class Point2D {
public:
	Point2D(T x, T y) {
		this->x = x;
		this->y = y;
	}
	T getX() {
		return x;
	}
	T getY() {
		return y;
	}
	void setX(T x) {
		this->x = x;
	}
	void setY(T y) {
		this->y = y;
	}
private:
	T x;
	T y;
};