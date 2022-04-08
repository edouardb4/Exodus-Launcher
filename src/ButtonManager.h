#pragma once
#include "Button.h"

class ButtonManager {

private:
	std::shared_ptr<std::list<std::shared_ptr<Button>>> buttons;

public:
	ButtonManager();
	void addButton(std::shared_ptr<Button> button);
	void removeButton(std::shared_ptr<Button> button);
	std::shared_ptr<std::list<std::shared_ptr<Button>>> getButtons();
};