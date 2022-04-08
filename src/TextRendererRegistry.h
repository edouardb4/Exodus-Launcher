#pragma once

#include <string>
#include <unordered_map>
#include "TextRenderer.h"
class TextRendererRegistry {

private:
	std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<TextRenderer>>> registered;

public:
	TextRendererRegistry();

	bool isBinded(std::string cmp);
	void bind(std::string toBind, std::shared_ptr<TextRenderer> shader);
	void unbind(std::string toUnbind);
	std::shared_ptr<TextRenderer> lookup(std::string toFind);
	std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<TextRenderer>>> getRegistered() {
		return registered;
	}
};