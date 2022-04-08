
#include "PanelComponent.h"

PanelComponent::PanelComponent(Point2D<float> coordinates) : coordinates(coordinates) {
}
void PanelComponent::draw(float mouseX, float mouseY) {

}
void PanelComponent::setPaddingWidth(float paddingWidth) {
	this->paddingWidth = paddingWidth;
}
void PanelComponent::setPaddingHeight(float paddingHeight) {
	this->paddingHeight = paddingHeight;
}
float PanelComponent::getPaddingWidth() {
	return paddingWidth;
}
float PanelComponent::getPaddingHeight() {
	return paddingHeight;
}
void PanelComponent::setCoordinates(Point2D<float> coordinates) {
	this->coordinates = coordinates;
}
Point2D<float> PanelComponent::getCoordinates() {
	return coordinates;
}
glm::vec4 PanelComponent::getBorderColor() {
	return borderColor;
}
glm::vec4 PanelComponent::getColor() {
	return color;
}
void PanelComponent::setColor(glm::vec4 color) {
	this->color = color;
}
void PanelComponent::setBorderColor(glm::vec4 borderColor) {
	this->borderColor = borderColor;
}
void PanelComponent::setWidth(float width) {
	this->width = width;
}
void PanelComponent::setHeight(float height) {
	this->height = height;
}
float PanelComponent::getHeight() {
	return height;
}
float PanelComponent::getWidth() {
	return width;
}