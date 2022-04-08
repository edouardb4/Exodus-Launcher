#pragma once

#include <glm/vec4.hpp>

#include "Point2D.h"

class PanelComponent {
public:
	PanelComponent(Point2D<float> coordinates);
	virtual void draw(float mouseX, float mouseY);
	void setPaddingWidth(float paddingWidth);
	void setPaddingHeight(float paddingHeight);
	float getPaddingWidth();
	float getPaddingHeight();
	void setWidth(float width);
	void setHeight(float height);
	float getHeight();
	float getWidth();
	void setCoordinates(Point2D<float> coordinates);
	virtual Point2D<float> getCoordinates();
	glm::vec4 getBorderColor();
	glm::vec4 getColor();
	void setColor(glm::vec4 color);
	void setBorderColor(glm::vec4 borderColor);
protected:
	float width;
	float height;
	Point2D<float> coordinates;
	float paddingWidth;
	float paddingHeight;
	glm::vec4 borderColor;
	glm::vec4 color;
};