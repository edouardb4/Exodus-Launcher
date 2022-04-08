#include "ShaderRegistry.h"

ShaderRegistry::ShaderRegistry() {
	registered = std::make_shared<std::unordered_map<std::string, std::shared_ptr<Shader>>>();
}

bool ShaderRegistry::isBinded(std::string cmp) {
	return getRegistered()->count(cmp) > 0;
}

void ShaderRegistry::bind(std::string toBind, std::shared_ptr<Shader> shader) {
	getRegistered()->insert({ toBind, shader });
}

void ShaderRegistry::unbind(std::string toUnbind) {
	getRegistered()->erase(toUnbind);
}

std::shared_ptr<Shader> ShaderRegistry::lookup(std::string toFind) {
	return getRegistered()->at(toFind);
}