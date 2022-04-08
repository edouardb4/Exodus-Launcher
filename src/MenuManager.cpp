#include "MenuManager.h"

bool MenuManager::isPresent(std::string name) {
	return menus->count(name) > 0;
}

std::shared_ptr<Menu> MenuManager::getMenu(std::string name) {
	return menus->at(name);
}

void MenuManager::addMenu(std::string name, std::shared_ptr<Menu> menu) {
	menus->insert({ name, menu });
}

void MenuManager::removeMenu(std::string name) {
	menus->erase(name);
}

std::unordered_map<std::string, std::shared_ptr<Menu>>* MenuManager::getMenus() {
	return menus.get();
}

MenuManager::MenuManager() {
	this->menus = std::make_shared<std::unordered_map<std::string, std::shared_ptr<Menu>>>();
}