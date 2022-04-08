#include "ButtonAnimation.h"

ButtonAnimation::ButtonAnimation(std::shared_ptr<SystemInformation> systemInformation) {
	this->systemInformation = systemInformation;
}

void ButtonAnimation::terminate() {
	//IMPLEMENT
}

void ButtonAnimation::run(float normalized) {
	//IMPLEMENT
}

float ButtonAnimation::getNormalized() {
	return 0.0f;
}

bool ButtonAnimation::canTerminate(float normalized) {
	return normalized > 1.0f;
}