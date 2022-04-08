#include "ButtonComponent.h"

ButtonComponent::ButtonComponent(std::shared_ptr<Button> button) : PanelComponent(Point2D<float>(0.0f, 0.0f)), button(button) {
}

Button* ButtonComponent::getButton() {
	return button.get();
}

Point2D<float> ButtonComponent::getCoordinates() {
	return Point2D<float>(button->getX(), button->getY());
}

void ButtonComponent::draw(float mouseX, float mouseY) {
	getButton()->drawButton(mouseX, mouseY);
}