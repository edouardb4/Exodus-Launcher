#pragma once

class ScreenResolution {
private:
	int width, height;
public:
	ScreenResolution(int height, int width);
	int getHeight();
	int getWidth();
	int getResolutionHeight();
	int getResolutionWidth();
};