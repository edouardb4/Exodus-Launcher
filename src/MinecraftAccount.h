#pragma once

#include <string>
#include "json.hpp"

using nlohmann::json;

class MinecraftAccount {
public:
	MinecraftAccount();
	MinecraftAccount(std::string userCredentials, std::string uuid, std::string accessToken);
	std::string getUserCredentials() { return userCredentials; }
	std::string getUUID() { return uuid; }
	std::string getAccessToken() { return accessToken; }
	void to_json(json& j, const MinecraftAccount& manager);
	void from_json(const json& j, MinecraftAccount& manager);
private:
	std::string userCredentials;
	std::string uuid;
	std::string accessToken;
};