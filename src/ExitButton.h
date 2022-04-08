#pragma once
#include "Button.h"

class ExitButton : public Button {
protected:
	glm::vec4 color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
public:
	ExitButton(std::shared_ptr<Renderer> renderer, float x, float y, float width, float height);
	virtual void drawButton(float mouseX, float mouseY);
	void setDrawColor(glm::vec4 color);
};