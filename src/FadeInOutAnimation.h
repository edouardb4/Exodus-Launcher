#pragma once

#include "Button.h"
#include "ButtonAnimation.h"

class FadeInOutAnimation : public ButtonAnimation {
protected:
	std::shared_ptr<Button> btn;
	glm::vec2 coordinates;
	unsigned int time;
	float scaleWidth, scaleHeight;
	float rawWidth, rawHeight;
	bool hovering, fade;
	long long startHover;
	long long startFade;
public:
	FadeInOutAnimation(std::shared_ptr<Button>, std::shared_ptr<SystemInformation> systemInformation, float scaleWidth, float scaleHeight, unsigned int time);
	virtual void terminate();
	virtual float getNormalized();
	virtual bool canTerminate(float normalized);
	virtual void run(float normalized);
};