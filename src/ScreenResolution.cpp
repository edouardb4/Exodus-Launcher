#include "ScreenResolution.h"

const static int RATIO_HEIGHT = 2; //Imaginary axis we're working with. Not real resolution.
const static int RATIO_WIDTH = 2; //Imaginary axis we're working with. Not real resolution.

ScreenResolution::ScreenResolution(int width, int height) : width(width), height(height) {

}

int ScreenResolution::getHeight() {
	return RATIO_HEIGHT;
}

int ScreenResolution::getWidth() {
	return RATIO_WIDTH;
}

int ScreenResolution::getResolutionHeight() {
	return height;
}

int ScreenResolution::getResolutionWidth() {
	return width;
}