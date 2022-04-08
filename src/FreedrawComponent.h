#pragma once

#include <functional>
#include "PanelComponent.h"

class FreedrawComponent : public PanelComponent {
protected:
	std::function<void(float, float)> drawCall;
public:
	FreedrawComponent(Point2D<float> coordinates);
	virtual void draw(float mouseX, float mouseY);
	template<typename Func>
	void setDrawCall(Func drawCall) {
		this->drawCall = drawCall;
	}
};