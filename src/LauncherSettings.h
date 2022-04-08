#pragma once
#include "LauncherType.h"
#include "json.hpp"

using json = nlohmann::json;

namespace LauncherSettings {
	struct Profile {
		unsigned long long dedicatedBytes = 0;
		int launcherType = 0;
		std::string activeClient;

		unsigned long long getDedicatedBytes();
		int getLauncherType();
		void setLauncherType(int);
		void setDedicatedBytes(unsigned long long);
		std::string getActiveClient();
		void setActiveClient(std::string activeClient);
	};

	Profile* loadFromFile(const std::string& const);
	void saveToFile(const Profile*, const std::string& const);
	void to_json(json&, const Profile&);
	void from_json(const json&, Profile&);
};