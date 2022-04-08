#pragma once

#include "ButtonModifier.h"

class PersistentGlow : public ButtonModifier {
private:
	std::shared_ptr<ScreenResolution> screenResolution;
	glm::vec3 color;
	float intensity;
	float scaleWidth, scaleHeight;
public:
	PersistentGlow(std::shared_ptr<ScreenResolution> screenResolution, glm::vec3 color, float intensity, float scaleWidth, float scaleHeight);
	virtual void doEffect(Button* button);
};