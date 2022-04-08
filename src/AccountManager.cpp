
#include "AccountManager.h"

#include <vector>
#include "MinecraftAccount.h"

using Minecraft::AccountManager;

void Minecraft::to_json(json& j, const AccountManager& manager) {
	std::vector<json> vector;
	for (std::shared_ptr<MinecraftAccount> account : manager.accounts) {
		json j;
		account->to_json(j, *account.get());
		vector.push_back(j);
	}
	j = json{ {"accounts", vector} };
}

void Minecraft::from_json(const json& j, AccountManager& manager) {
	std::vector<json> vector;
	j.at("accounts").get_to(vector);
	std::vector<std::shared_ptr<MinecraftAccount>> list;
	for (json j : vector) {
		std::shared_ptr<MinecraftAccount> account = std::make_shared<MinecraftAccount>();
		MinecraftAccount& ref = *account.get();
		ref.from_json(j, ref);
		list.push_back(account);
	}
	manager.accounts = list;
}