#pragma once

#include "Button.h"
#include "ExitButton.h"

class ButtonFactory {
public:
	virtual std::shared_ptr<Button> create(std::shared_ptr<Renderer> renderer, float x, float y, float width, float height, glm::vec4 color) {
		std::shared_ptr<Button> button = std::make_shared<Button>(renderer, x, y, width, height);
		button->setColorScheme(color);
		return button;
	}
};

class ExitButtonFactory : ButtonFactory {
	virtual std::shared_ptr<Button> create(std::shared_ptr<Renderer> renderer, float x, float y, float width, float height, glm::vec4 color) {
		std::shared_ptr<ExitButton> button = std::make_shared<ExitButton>(renderer, x, y, width, height);
		button->setColorScheme(color);
		return button;
	}
};