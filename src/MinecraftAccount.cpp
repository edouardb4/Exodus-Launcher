#include "MinecraftAccount.h"

MinecraftAccount::MinecraftAccount() {}

MinecraftAccount::MinecraftAccount(std::string userCredentials, std::string uuid, std::string accessToken) : userCredentials(userCredentials), uuid(uuid), accessToken(accessToken) {
}

void MinecraftAccount::to_json(json& j, const MinecraftAccount& account) {
	j = json{ {"usercredentials", account.userCredentials}, {"uuid", uuid }, {"accesstoken", account.accessToken} };
}

void MinecraftAccount::from_json(const json& j, MinecraftAccount& account) {
	j.at("usercredentials").get_to(account.userCredentials);
	j.at("uuid").get_to(uuid);
	j.at("accesstoken").get_to(accessToken);
}