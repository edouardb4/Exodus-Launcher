#include "FadeInOutAnimation.h"
#include "MathUtils.h"

#include <chrono>
#include <cstdint>

using namespace std::chrono;

FadeInOutAnimation::FadeInOutAnimation(std::shared_ptr<Button> btn, std::shared_ptr<SystemInformation> systemInformation, float scaleWidth, float scaleHeight, unsigned int time) :
	ButtonAnimation(systemInformation), btn(btn), time(time), scaleWidth(scaleWidth), scaleHeight(scaleHeight), rawWidth(btn->getWidth()), rawHeight(btn->getHeight()) {
	this->coordinates = glm::vec2(btn->getX(), btn->getY());
}

//Should never terminate. We assume this is always active.
bool FadeInOutAnimation::canTerminate(float normalized) {
	return false;
}

void FadeInOutAnimation::terminate() {
	this->btn->setWidth(rawWidth);
	this->btn->setHeight(rawHeight);
	this->btn->setX(coordinates[0]);
	this->btn->setY(coordinates[1]);
}

float FadeInOutAnimation::getNormalized() {
	long long systime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	float i = systime - startHover;
	return MathUtils::min(1.0f, MathUtils::max(0.0f, i / (float) time));
}

void FadeInOutAnimation::run(float normalized) {
	signed long long systime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	if (this->btn->isBound(this->systemInformation.get()->getMouseX(), this->systemInformation.get()->getMouseY())) {
		if (!hovering) {
			if (fade && startFade - systime > 0) {
				startHover = systime - (long long) (time * MathUtils::max(0.0f, MathUtils::min(1.0f, (float)(startFade - systime) / time)));
				fade = false;
			}
			else {
				startHover = systime;
			}
			hovering = true;
			return;
		}
	}
	else if (hovering) {
		hovering = false;
		fade = true;
		startFade = systime + (long long) (normalized * time);
	}
	else if (!hovering && fade) {
		normalized = MathUtils::max(0.0f, MathUtils::min(1.0f, (float) (startFade - systime) / time));
		if (normalized <= 0.0f) {
			fade = false;
		}
	}
	else {
		return;
	}
	float width = normalized * scaleWidth;
	this->btn->setWidth(width + rawWidth);
	float height = normalized * scaleHeight;
	this->btn->setHeight(height + rawHeight);
	this->btn->setX(coordinates[0] - width / 2);
	this->btn->setY(coordinates[1] - height / 2);
}