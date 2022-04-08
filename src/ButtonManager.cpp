#include "ButtonManager.h"

ButtonManager::ButtonManager() {
	this->buttons = std::make_shared<std::list<std::shared_ptr<Button>>>();
}

void ButtonManager::addButton(std::shared_ptr<Button> button) {
	buttons->push_back(button);
}

void ButtonManager::removeButton(std::shared_ptr<Button> button) {
	buttons->remove(button);
}

std::shared_ptr<std::list<std::shared_ptr<Button>>> ButtonManager::getButtons() {
	return buttons;
}