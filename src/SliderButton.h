#pragma once

#include "Button.h"
#include "ScreenResolution.h"

enum SliderType {
	CIRCLE,
	SQUARE
};

class SliderButton : public Button {
public:
	SliderButton(std::shared_ptr<ScreenResolution> resolution, std::shared_ptr<Renderer> renderer, float x, float y, float width, float height);
	virtual void drawButton(float mouseX, float mouseY);
	void setSliderPosition(float position);
	void setSliderType(int sliderType);
	int getSliderType();
	void setSliderColorScheme(glm::vec4 sliderColorScheme);
	void setSliderRadius(float sliderRadius);
	void setBarColorScheme(glm::vec4 barColorScheme);
	void setBarHeight(float barHeight);
	void setMouseDown(bool mouseDown);
	bool isMouseDown();
	glm::vec4 getSliderColorScheme();
	float getSliderPosition();
	std::function<void()> getReleaseFunction() {
		return releaseFunction;
	}
	std::function<void()> getHoldFunction() {
		return holdFunction;
	}
	template<typename Func>
	void setReleaseFunction(Func releaseFunction) {
		this->releaseFunction = releaseFunction;
	}
	template<typename Func>
	void setHoldFunction(Func holdFunction) {
		this->holdFunction = holdFunction;
	}
private:
	std::shared_ptr<ScreenResolution> resolution;
	bool mouseDown;
	float sliderRadius = 0.25f;
	float barHeight = 0.05f;
	float sliderPosition;
	int sliderType;
	glm::vec4 sliderColorScheme = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec4 barColorScheme;
	std::function<void()> holdFunction;
	std::function<void()> releaseFunction;
};