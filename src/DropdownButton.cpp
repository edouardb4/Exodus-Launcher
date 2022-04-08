#include "DropdownButton.h"
#include "CurveType.h"
#include "CurveMask.h"

DropdownButton::DropdownButton(std::shared_ptr<Renderer> renderer, float x, float y, float width, float height) : Button(renderer, x, y, width, height) {
	this->extendType == ExtendType::CLICK;
}

void DropdownButton::drawButton(float mouseX, float mouseY) {
	if (extendType == ExtendType::HOVER && isBound(mouseX, mouseY) || extended) {
		return;
	}
	drawDefaultButton(mouseX, mouseY);
}