#pragma once

#include <memory>
#include <vector>
#include "MinecraftAccount.h"
#include "json.hpp"

using json = nlohmann::json;

namespace Minecraft {
	class AccountManager {
	public:
		AccountManager() { }
	public:
		std::vector<std::shared_ptr<MinecraftAccount>> accounts;
	};
	void to_json(json& j, const AccountManager& settings);
	void from_json(const json& j, AccountManager& settings);
	AccountManager* loadFromFile(const std::string& const);
	void saveToFile(const AccountManager*, const std::string& const);
}