#pragma once

#include <string.h>
#include <unordered_map>
#include <glm/gtc/type_ptr.hpp>
#include "ShaderProgramSource.h"

class Shader {
private:
	std::string m_FilePath;
	unsigned int m_RendererID;
	std::unordered_map<std::string, int> m_UniformLocationCache;

public:
	Shader(const std::string& filePath);
	~Shader();

	void bind() const;
	void unbind() const;
	void setUniform1f(const std::string& name, float v0);
	void setUniform1i(const std::string& name, int v0);
	void setUniform2f(const std::string& name, float v0, float v1);
	void setUniform3f(const std::string& name, float v0, float v1, float v2);
	void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void setProjection(const glm::mat4 a_Projection);

	int getRendererId();

private:
	ShaderProgramSource parseShader(const std::string& filePath);
	int getUniformLocation(const std::string& name);
	unsigned int compileShader(unsigned int type, const std::string& source);
	unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);
};