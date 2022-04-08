#include "ExitButton.h"
#include "CurveType.h"
#include "CurveMask.h"

ExitButton::ExitButton(std::shared_ptr<Renderer> renderer, float x, float y, float width, float height) : Button(renderer, x, y, width, height) {
}

void ExitButton::drawButton(float mouseX, float mouseY) {
	ButtonType type = getButtonType();
	glm::vec4 colorToUse = isBound(mouseX, mouseY) ? hoverColorScheme : color;
	drawDefaultButton(mouseX, mouseY);
	float centerX = x + width / 2;
	float centerY = y + width / 2;
	renderer->drawLine(centerX - width / 8, centerY - height / 8, centerX + width / 8, centerY + height / 8, 2.0f, colorToUse);
	renderer->drawLine(centerX + width / 8, centerY - height / 8, centerX - width / 8, centerY + height / 8, 2.0f, colorToUse);
}

void ExitButton::setDrawColor(glm::vec4 color) {
	this->color = color;
}