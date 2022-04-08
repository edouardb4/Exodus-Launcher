#pragma once

#include <list>
#include "Menu.h"

class MenuManager {
public:
	MenuManager();
	bool isPresent(std::string name);
	std::shared_ptr<Menu> getMenu(std::string name);
	void addMenu(std::string name, std::shared_ptr<Menu> menu);
	void removeMenu(std::string name);
	std::unordered_map<std::string, std::shared_ptr<Menu>>* getMenus();

private:
	std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<Menu>>> menus;
};