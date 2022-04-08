#include <GL/glew.h>
#include <GL/GL.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include "Renderer.h"
#include "MathUtils.h"
#include "CurveType.h"
#include "CurveMask.h"

Renderer::Renderer(std::shared_ptr<TextRenderer> textRenderer, std::shared_ptr<Shader> shader, std::shared_ptr<Shader> textShader, std::shared_ptr<Shader> textureShader) : textRenderer(textRenderer), shader(shader), textShader(textShader), textureShader(textureShader) {
}

void Renderer::color4f(float red, float green, float blue, float alpha) {
	shader->setUniform4f("u_Color", red, green, blue, alpha);
}

void Renderer::color3f(float red, float green, float blue) {
	color4f(red, green, blue, 1.0f);
}

void Renderer::drawText(float x, float y, std::string& text, float color[4]) {
	textRenderer->renderText(*textShader.get(), text, x, y, 1.0f, glm::vec3(color[0], color[1], color[2]));
}

void Renderer::drawText(float x, float y, std::string& text, glm::vec4 color) {
	textRenderer->renderText(*textShader.get(), text, x, y, 1.0f, glm::vec3(color[0], color[1], color[2]));
}

void Renderer::drawText(TextRenderer* textRenderer, float x, float y, std::string& text, glm::vec4 color) {
	textRenderer->renderText(*textShader.get(), text, x, y, 1.0f, glm::vec3(color[0], color[1], color[2]));
}

void Renderer::drawHollowRect(float x, float y, float width, float height, float angle, float lineWidth, glm::vec4 color) {
	shader->bind();
	glEnable(GL_BLEND);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glLineWidth(lineWidth);
	color4f(color[0], color[1], color[2], color[3]);
	float centerX = x + width / 2;
	float centerY = y + height / 2;
	float* vertices = (float*)malloc(sizeof(float) * 10);
	float rawWidth = width;
	float rawHeight = height;
	width *= 0.5f * (1.0f / cos(MathUtils::toRadians(45)));
	height *= 0.5f * (1.0f / sin(MathUtils::toRadians(45)));
	vertices[0] = centerX + width * cos(MathUtils::toRadians(angle + 45));
	vertices[1] = centerY + height * sin(MathUtils::toRadians(angle + 45));
	vertices[2] = centerX + width * cos(MathUtils::toRadians(angle + 135));
	vertices[3] = centerY + height * sin(MathUtils::toRadians(angle + 135));
	vertices[4] = centerX + width * cos(MathUtils::toRadians(angle + 225));
	vertices[5] = centerY + height * sin(MathUtils::toRadians(angle + 225));
	vertices[6] = centerX + width * cos(MathUtils::toRadians(angle + 315));
	vertices[7] = centerY + height * sin(MathUtils::toRadians(angle + 315));
	vertices[8] = vertices[0];
	vertices[9] = vertices[1];


	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 10, vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

	glDrawArrays(GL_LINE_LOOP, 0, 5);

	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
	free(vertices);

	shader->unbind();
	glDisable(GL_BLEND);
	glDisable(GL_LINE_SMOOTH);
}

void Renderer::drawHollowCurvedRect(float x, float y, float width, float height, int mask, float lineWidth, glm::vec4 color) {
	float curve = height / 5.0f;
	bool bottomLeft = CurveMask::isInMask(CurveType::BOTTOM_LEFT, mask);
	bool topLeft = CurveMask::isInMask(CurveType::TOP_LEFT, mask);
	bool topRight = CurveMask::isInMask(CurveType::TOP_RIGHT, mask);
	bool bottomRight = CurveMask::isInMask(CurveType::BOTTOM_RIGHT, mask);
	if (bottomLeft) {
		drawHollowCone(x + curve, y + curve, x + curve, y + curve, curve, curve, 180, lineWidth, color);
	}
	else {
		drawLine(x, y, x, y + curve, lineWidth, color);
		drawLine(x, y, x + curve, y, lineWidth, color);
	}
	if (topLeft) {
		drawHollowCone(x + curve, y + height - curve, x + curve, y + height - curve, curve, curve, 90, lineWidth, color);
	}
	else {
		drawLine(x, y + height - curve, x, y + height, lineWidth, color);
		drawLine(x, y + height, x + curve, y + height, lineWidth, color);
	}
	if (topRight) {
		drawHollowCone(x + width - curve, y + height - curve, x + width - curve, y + height - curve, curve, curve, 0, lineWidth, color);
	}
	else {
		drawLine(x + width - curve, y + height, x + width, y + height, lineWidth, color);
		drawLine(x + width, y + height - curve, x + width, y + height, lineWidth, color);
	}
	if (bottomRight) {
		drawHollowCone(x + width - curve, y + curve, x + width - curve, y + curve, curve, curve, 270, lineWidth, color);
	}
	else {
		drawLine(x + width - curve, y, x + width, y, lineWidth, color);
		drawLine(x + width, y, x + width, y + curve, lineWidth, color);
	}
	drawLine(x + curve, y + height, x + width - curve, y + height, lineWidth, color);
	drawLine(x, y + curve, x, y + height - curve, lineWidth, color);
	drawLine(x + curve, y, x + width - curve, y, lineWidth, color);
	drawLine(x + width, y + curve, x + width, y + height - curve, lineWidth, color);
}

std::shared_ptr<TextRenderer> Renderer::getTextRenderer() {
	return textRenderer;
}

void Renderer::drawHollowCone(float xRotate, float yRotate, float x, float y, float width, float height, float angle, float lineWidth, glm::vec4 color) {
	float pvx = NULL;
	float pvy = NULL;
	float step = 5.0f;
	for (float i = angle; i <= angle + 90; i += step) {
		float px = xRotate + width * cos(MathUtils::toRadians(i));
		float py = yRotate + height * sin(MathUtils::toRadians(i));
		if (pvx != NULL) {
			drawLine(px, py, pvx, pvy, lineWidth, color);
		}
		pvx = px;
		pvy = py;
	}
}

int countIndividualBits(int mask) {
	int i = 0;
	int j = 1;
	int k = 0;
	int to = 1 << 31;
	while (j != to) {
		j = 1 << i++;
		if (mask & j) {
			k++;
		}
	}
	return k;
}

void Renderer::renderImage(float width, float height, float x, float y, Texture& texture, float imageVertices[8]) {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	textureShader->bind();
	textureShader->setUniform1i("u_Texture", 0);
	texture.bind();
	float positions[16] = {
		x, y, imageVertices[0], imageVertices[1],
		x, y + height, imageVertices[2], imageVertices[3],
		x + width, y + height, imageVertices[4], imageVertices[5],
		x + width, y, imageVertices[6], imageVertices[7]
	};
	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 16, positions, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (const void*)(sizeof(float) * 2));

	unsigned int IBO;
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);


	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

	glDisable(GL_BLEND);
	texture.unbind();
	textureShader->unbind();
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &IBO);
}

void Renderer::renderImage(float width, float height, float x, float y, Texture& texture) {
	float vertices[8] = { 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f };
	renderImage(width, height, x, y, texture, vertices);
}

void Renderer::drawCone(float xRotate, float yRotate, float x, float y, float width, float height, float angle, glm::vec4 color) {
	shader->bind();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	color4f(color[0], color[1], color[2], color[3]);
	float step = 0.1f;
	std::vector<float> list;
	list.push_back(x);
	list.push_back(y);
	for (float i = angle; i <= angle + 90; i += step) {
		float px = xRotate + width * cos(MathUtils::toRadians(i));
		float py = yRotate + height * sin(MathUtils::toRadians(i));
		list.push_back(px);
		list.push_back(py);
	}
	float* vertices = (float*)malloc(sizeof(float) * list.size());
	for (int i = 0; i < list.size(); i++) {
		float element = list[i];
		vertices[i] = element;
	}
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * list.size(), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

	glDrawArrays(GL_TRIANGLE_FAN, 0, list.size() / 2);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
	glDisable(GL_SMOOTH);
	glDisable(GL_BLEND);
	free(vertices);

	shader->unbind();
}

void Renderer::drawCurvedRect(float x, float y, float width, float height, int mask, glm::vec4 color) {
	float curve = height / 5.0f;
	bool bottomLeft = CurveMask::isInMask(CurveType::BOTTOM_LEFT, mask);
	bool topLeft = CurveMask::isInMask(CurveType::TOP_LEFT, mask);
	bool topRight = CurveMask::isInMask(CurveType::TOP_RIGHT, mask);
	bool bottomRight = CurveMask::isInMask(CurveType::BOTTOM_RIGHT, mask);

	if (bottomLeft) {
		drawCone(x + curve, y + curve, x + curve, y + curve, curve, curve, 180, color);
	}
	else {
		drawRect(x, y, curve, curve, color);
	}
	if (topLeft) {
		drawCone(x + curve, y + height - curve, x + curve, y + height - curve, curve, curve, 90, color);
	}
	else {
		drawRect(x, y + height - curve, curve, curve, color);
	}
	if (topRight) {
		drawCone(x + width - curve, y + height - curve, x + width - curve, y + height - curve, curve, curve, 0, color);
	}
	else {
		drawRect(x + width - curve, y + height - curve, curve, curve, color);
	}
	if (bottomRight) {
		drawCone(x + width - curve, y + curve, x + width - curve, y + curve, curve, curve, 270, color);
	}
	else {
		drawRect(x + width - curve, y, curve, curve, color);
	}

	drawRect(x + curve, y + curve, width - curve * 2, height - curve * 2, color);
	drawRect(x, y + curve, curve, height - curve * 2, color);
	drawRect(x + width - curve, y + curve, curve, height - curve * 2, color);
	drawRect(x + curve, y, width - curve * 2, curve, color);
	drawRect(x + curve, y + height - curve, width - curve * 2, curve, color);
}

void Renderer::drawAbsCurve(float x, float y, float width, float height, float lineWidth, glm::vec4 color) {
	drawLine(x, y, x - width, x + height, lineWidth, color);
	drawLine(x, y, x + width, y + height, lineWidth, color);
}

void Renderer::drawRect(float x, float y, float width, float height, float rgba[4]) {
	shader->bind();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	color4f(rgba[0], rgba[1], rgba[2], rgba[3]);
	float vertices[8] = {
		x, y,
		x, y + height,
		x + width, y + height,
		x + width, y
	};
	unsigned int indices[6] = {
		0, 1, 2,
		0, 2, 3
	};
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

	unsigned int IBO;
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * 2 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &IBO);

	glDisable(GL_BLEND);
	shader->unbind();
}

void Renderer::drawTriangle(float x, float y, float width, float height, glm::vec4 color) {
	shader->bind();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	color4f(color[0], color[1], color[2], color[3]);
	float vertices[6] = {
		x, y,
		x - width, y,
		x, y - height
	};
	unsigned int indices[3] = {
		0, 1, 2
	};
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

	unsigned int IBO;
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * 1 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &IBO);

	glDisable(GL_BLEND);
	shader->unbind();
}

void Renderer::drawRect(float x, float y, float width, float height, glm::vec4 rgba) {
	float buffer[4] = { rgba[0], rgba[1], rgba[2], rgba[3] };
	drawRect(x, y, width, height, buffer);
}

void Renderer::drawLine(float x, float y, float x1, float y1, float lineWidth, glm::vec4 color) {
	shader->bind();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_SMOOTH);
	glLineWidth(lineWidth);
	color4f(color[0], color[1], color[2], color[3]);
	float* vertices = (float*)malloc(sizeof(float) * 4);
	vertices[0] = x;
	vertices[1] = y;
	vertices[2] = x1;
	vertices[3] = y1;
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(float), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

	glDrawArrays(GL_LINES, 0, 4);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
	glDisable(GL_SMOOTH);
	glDisable(GL_BLEND);
	free(vertices);

	shader->unbind();
}

void Renderer::drawEllipse(float x, float y, float width, float height, glm::vec4 color) {
	drawEllipse(x, y, width, height, 0, 360, color);
}

void Renderer::drawEllipse(float x, float y, float width, float height, float angleFrom, float angleTo, glm::vec4 color) {
	shader->bind();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	color4f(color[0], color[1], color[2], color[3]);
	std::vector<float> vertices;
	float step = 0.1f;
	vertices.push_back(x);
	vertices.push_back(y);
	for (float i = angleFrom; i <= angleTo; i += step) {
		vertices.push_back(x + cos(MathUtils::toRadians(i)) * width / 2.0f);
		vertices.push_back(y + sin(MathUtils::toRadians(i)) * height / 2.0f);
	}
	float* buffer = (float*)malloc(sizeof(float) * vertices.size());
	for (int i = 0; i < vertices.size(); i++) {
		buffer[i] = vertices[i];
	}
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), buffer, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

	glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size() / 2);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glDisable(GL_BLEND);

	free(buffer);
	shader->unbind();
}

void Renderer::drawCircle(float x, float y, float radius, float angleFrom, float angleTo, float color[4]) {
	shader->bind();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	color4f(color[0], color[1], color[2], color[3]);
	std::vector<float> vertices;
	float step = 0.1f;
	vertices.push_back(x);
	vertices.push_back(y);
	for (float i = angleFrom; i <= angleTo; i += step) {
		vertices.push_back(x + cos(MathUtils::toRadians(i)) * radius);
		vertices.push_back(y + sin(MathUtils::toRadians(i)) * radius);
	}
	float* buffer = (float*)malloc(sizeof(float) * vertices.size());
	for (int i = 0; i < vertices.size(); i++) {
		buffer[i] = vertices[i];
	}
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), buffer, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

	glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size() / 2);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glDisable(GL_BLEND);

	free(buffer);
	shader->unbind();
}

void Renderer::drawCircle(float x, float y, float radius, float angle, float rgba[4]) {
	drawCircle(x, y, radius, angle, angle + 90, rgba);
}