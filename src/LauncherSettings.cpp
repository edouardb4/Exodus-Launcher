#include "LauncherSettings.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>  
#include <Windows.h>
#include <filesystem>

using namespace LauncherSettings;

unsigned long long Profile::getDedicatedBytes() {
	return dedicatedBytes;
}

int Profile::getLauncherType() {
	return launcherType;
}

void Profile::setDedicatedBytes(unsigned long long dedicatedBytes) {
	this->dedicatedBytes = dedicatedBytes;
}

void Profile::setLauncherType(int launcherType) {
	this->launcherType = launcherType;
}

std::string Profile::getActiveClient() {
	if (activeClient == "") {
		return "Exodus Client";
	}
	return activeClient;
}
void Profile::setActiveClient(std::string activeClient) {
	this->activeClient = activeClient;
}

void LauncherSettings::to_json(json& j, const Profile& settings) {
	j = json{ {"launchertype", settings.launcherType}, {"dedicatedbytes", settings.dedicatedBytes}, {"activeclient", settings.activeClient} };
}
void LauncherSettings::from_json(const json& j, Profile& settings) {
	j.at("dedicatedbytes").get_to(settings.dedicatedBytes);
	j.at("launchertype").get_to(settings.launcherType);
	if (j.contains("activeclient")) {
		j.at("activeclient").get_to(settings.activeClient);
	}
}

Profile* LauncherSettings::loadFromFile(const std::string& const fileName) {
	std::ifstream stream(fileName);
	Profile* profile = new Profile();
	if (!stream.good()) {
		return profile;
	}
	json j;
	stream >> j;
	from_json(j, *profile);
	return profile;
}

void LauncherSettings::saveToFile(const Profile* profile, const std::string& const fileName) {
	json j;
	to_json(j, *profile);
	std::ofstream stream(fileName);
	stream << std::setw(4) << j << std::endl;
}