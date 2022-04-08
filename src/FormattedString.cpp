#include "FormattedString.h"

bool replace(std::string& str, const std::string& from, const std::string& to) {
	size_t start_pos = str.find(from);
	if (start_pos == std::string::npos)
		return false;
	str.replace(start_pos, from.length(), to);
	return true;
}

std::string FormattedString::getText() {
	return this->text;
}

std::string FormattedString::getFormattedText(std::unordered_map<std::string, std::string> map) {
	std::string text = getText();
	for (std::unordered_map<std::string, std::string>::iterator it = map.begin(); it != map.end(); it++) {
		replace(text, it->first, it->second);
	}
	return text;
}