#pragma once

#include "PanelComponent.h"
#include "Button.h"

class ButtonComponent : public PanelComponent {
public:
	ButtonComponent(std::shared_ptr<Button> button);
	Button* getButton();
	virtual Point2D<float> getCoordinates();
	virtual void draw(float mouseX, float mouseY);
private: 
	std::shared_ptr<Button> button;
};