#pragma once
#include "Shader.h"
#include "TextRenderer.h"
#include "Renderer.h"
#include "Texture.h"

class Renderer {
private:
	std::shared_ptr<Shader> shader;
	std::shared_ptr<Shader> textShader;
	std::shared_ptr<Shader> textureShader;
	std::shared_ptr<TextRenderer> textRenderer;
public:
	Renderer(std::shared_ptr<TextRenderer> textRenderer, std::shared_ptr<Shader> shader, std::shared_ptr<Shader> textShader, std::shared_ptr<Shader> textureShader);
	std::shared_ptr<TextRenderer> getTextRenderer();
	void drawEllipse(float x, float y, float width, float height, glm::vec4 color);
	void drawEllipse(float x, float y, float width, float height, float angleFrom, float angleTo, glm::vec4 color);
	void drawCircle(float x, float y, float radius, float angle, float color[4]);
	void drawCircle(float x, float y, float radius, float angleFrom, float angleTo, float color[4]);
	void drawText(float x, float y, std::string& text, float color[4]);
	void drawText(float x, float y, std::string& text, glm::vec4 color);
	void drawTriangle(float x, float y, float width, float height, glm::vec4 color);
	void drawText(TextRenderer* textRenderer, float x, float y, std::string& text, glm::vec4 color);
	void drawRect(float x, float y, float width, float height, float rgba[4]);
	void drawRect(float x, float y, float width, float height, glm::vec4 rgba);
	void drawLine(float x, float y, float x1, float y1, float lineWidth, glm::vec4 color);
	void drawCurvedRect(float x, float y, float width, float height, int mask, glm::vec4 color);
	void renderImage(float width, float height, float x, float y, Texture& texture);
	void renderImage(float width, float height, float x, float y, Texture& texture, float imageVertices[8]);
	void drawCone(float xRotate, float yRotate, float x, float y, float width, float height, float angle, glm::vec4 color);
	void drawHollowCurvedRect(float x, float y, float width, float height, int mask, float lineWidth, glm::vec4 color);
	void drawHollowRect(float x, float y, float width, float height, float angle, float lineWidth, glm::vec4 color);
	void drawHollowCone(float xRotate, float yRotate, float x, float y, float width, float height, float angle, float lineWidth, glm::vec4 color);
	void drawAbsCurve(float x, float y, float width, float height, float lineWidth, glm::vec4 color);
	void color4f(float red, float green, float blue, float alpha);
	void color3f(float red, float green, float blue);
};