#include "SliderButton.h"

SliderButton::SliderButton(std::shared_ptr<ScreenResolution> resolution, std::shared_ptr<Renderer> renderer, float x, float y, float width, float height)
	: resolution(resolution), sliderType(SliderType::CIRCLE), Button(renderer, x, y, width, height) {
	SliderButton* btn = this;
	mouseDown = false;
	holdFunction = [btn]() {
		btn->mouseDown = true;
	};
	releaseFunction = [btn]() {
		btn->mouseDown = false;
	};
}

void SliderButton::setMouseDown(bool mouseDown) {
	this->mouseDown = mouseDown;
}

void SliderButton::drawButton(float mouseX, float mouseY) {
	drawDefaultButton(mouseX, mouseY);
	float posx = this->sliderPosition * width + x;
	float posy = this->y + height / 2;
	renderer->drawRect(this->x, this->y + height / 2, width, height * sliderRadius / 4.0f, barColorScheme);
	if (sliderType == (int)SliderType::CIRCLE) {
		float ratio = (float)resolution->getResolutionWidth() / (float)resolution->getResolutionHeight();
		renderer->drawEllipse(posx, posy + sliderRadius * height / 4.0f, sliderRadius * height, sliderRadius * height * ratio, sliderColorScheme);
	}
	else if (sliderType == SliderType::SQUARE) {
		renderer->drawRect(posx, posy - sliderRadius * height / 4.0f, sliderRadius * height, sliderRadius * height, sliderColorScheme);
	}
}

void SliderButton::setBarHeight(float barHeight) {
	this->barHeight = barHeight;
}

void SliderButton::setBarColorScheme(glm::vec4 barColorScheme) {
	this->barColorScheme = barColorScheme;
}

bool SliderButton::isMouseDown() {
	return mouseDown;
}

void SliderButton::setSliderRadius(float sliderRadius) {
	this->sliderRadius = sliderRadius;
}

void SliderButton::setSliderPosition(float sliderPosition) {
	this->sliderPosition = sliderPosition;
}

float SliderButton::getSliderPosition() {
	return sliderPosition;
}

void SliderButton::setSliderColorScheme(glm::vec4 sliderColorScheme) {
	this->sliderColorScheme = sliderColorScheme;
}
glm::vec4 SliderButton::getSliderColorScheme() {
	return sliderColorScheme;
}

void SliderButton::setSliderType(int sliderType) {
	this->sliderType = sliderType;
}

int SliderButton::getSliderType() {
	return sliderType;
}