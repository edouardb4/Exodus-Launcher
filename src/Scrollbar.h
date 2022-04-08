#pragma once

#include <memory>
#include <glm/glm.hpp>
#include "Point2D.h"
#include "Renderer.h"

class Scrollbar {
public:
	Scrollbar(float width, float height);
	void setCoordinates(Point2D<float> coordinates) { this->coordinates = coordinates; }
	void setFillColor(glm::vec4 fillColor) { this->fillColor = fillColor; }
	void setColor(glm::vec4 color) { this->color = color; }
	float getWidth() { return width; }
	float getHeight() { return height; }
	void setWidth(float width) { this->width = width; }
	void setHeight(float height) { this->height = height; }
	Point2D<float> getCoordinates() { return coordinates; }
	void setPosition(float position) { this->position = position; }
	float getPosition() { return position; }
	void draw(Renderer* renderer, float lowestCoordinate, float panelHeight, float panelCoordinate, float mouseX, float mouseY);
protected:
	Point2D<float> coordinates = Point2D<float>(1.0f, 1.0f);
	float width, height;
	float position;
	glm::vec4 color, fillColor;
};