#pragma once

#include <string>
#include <unordered_map>

class FormattedString {
public:
	std::string getText();
	std::string getFormattedText(std::unordered_map<std::string, std::string> map);
private:
	std::string text;
};

class ReplaceBuilder {
public:
	ReplaceBuilder& append(std::string key, std::string value) {
		replaceMap.insert({ key, value });
		return *this;
	}
private:
	std::unordered_map<std::string, std::string> replaceMap;
};