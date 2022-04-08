#pragma once

#include "Button.h"

class ButtonModifier {
public:
	virtual void doEffect(Button* button);
};