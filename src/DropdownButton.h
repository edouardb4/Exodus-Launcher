#pragma once
#include "Button.h"
#include "Panel.h"

enum ExtendType : uint8_t {
	HOVER,
	CLICK
};

class DropdownButton : public Button {
protected:
	glm::vec4 color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	std::shared_ptr<Panel> panel;
	uint8_t extendType;
	bool extended;
public:
	DropdownButton(std::shared_ptr<Renderer> renderer, float x, float y, float width, float height);
	virtual void drawButton(float mouseX, float mouseY);
	void setPanel(std::shared_ptr<Panel> panel) { this->panel = panel; }
	void setExtendType(uint8_t extendType) { this->extendType = extendType; }
};