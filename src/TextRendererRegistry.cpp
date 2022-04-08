#include "TextRendererRegistry.h"

TextRendererRegistry::TextRendererRegistry() {
	registered = std::make_shared<std::unordered_map<std::string, std::shared_ptr<TextRenderer>>>(std::unordered_map<std::string, std::shared_ptr<TextRenderer>>());
}

bool TextRendererRegistry::isBinded(std::string cmp) {
	return getRegistered()->count(cmp) > 0;
}

void TextRendererRegistry::bind(std::string toBind, std::shared_ptr<TextRenderer> shader) {
	getRegistered()->insert({ toBind, shader });
}

void TextRendererRegistry::unbind(std::string toUnbind) {
	getRegistered()->erase(toUnbind);
}

std::shared_ptr<TextRenderer> TextRendererRegistry::lookup(std::string toFind) {
	std::shared_ptr<TextRenderer> find = getRegistered()->at(toFind);
	return find;
}