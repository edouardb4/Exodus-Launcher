#pragma once

#include <string>
#include <unordered_map>
#include <memory>

#include "Shader.h"
class ShaderRegistry {

private:
	std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<Shader>>> registered;
public:
	ShaderRegistry();

	bool isBinded(std::string cmp);
	void bind(std::string toBind, std::shared_ptr<Shader> shader);
	void unbind(std::string toUnbind);
	std::shared_ptr<Shader> lookup(std::string toFind);
	std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<Shader>>> getRegistered() {
		return registered;
	}

};