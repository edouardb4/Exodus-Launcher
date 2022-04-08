#include "FreedrawComponent.h"

FreedrawComponent::FreedrawComponent(Point2D<float> coordinates) : PanelComponent(coordinates) {
}

void FreedrawComponent::draw(float mouseX, float mouseY) {
	drawCall(mouseX, mouseY);
}
