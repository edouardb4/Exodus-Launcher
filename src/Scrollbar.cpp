#include "Scrollbar.h"

Scrollbar::Scrollbar(float width, float height) : coordinates(Point2D<float>(0.0f, 0.0f)) {
	this->width = width;
	this->height = height;
	this->fillColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	this->color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	this->position = 0;
}

void Scrollbar::draw(Renderer* renderer, float lowestCoordinate, float panelHeight, float panelCoordinate, float mouseX, float mouseY) {
	renderer->drawRect(coordinates.getX(), coordinates.getY(), width, height, color);
	float cWidth = width;
	float ycmp = lowestCoordinate;
	float m = panelCoordinate + panelHeight;
	float diff = panelCoordinate - ycmp;
	float cHeight = panelHeight * 2.0f / (2.0f + diff);
	float pos = panelCoordinate + height - cHeight - (position * (height - cHeight));
	renderer->drawRect(coordinates.getX(), pos, cWidth, cHeight, fillColor);
}