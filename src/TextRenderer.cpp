#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "TextRenderer.h"

TextRenderer::TextRenderer(std::shared_ptr<ScreenResolution> resolution, const char* const fileName, int fontSize) : fileName(fileName), resolution(resolution) {
	loadFile(fileName, fontSize);
	loadGlyphs();
}


ScreenResolution& TextRenderer::getResolution() {
	ScreenResolution& ret = *resolution;
	return ret;
}

void TextRenderer::loadGlyphs() {
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction
	for (wchar_t c = 0; c < 200; c++)
	{
		// load character glyph 
		if (FT_Load_Char(this->face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
		// generate texture
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		// set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// now store character for later use
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		this->Characters.insert({ c, character });
	}
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

void TextRenderer::loadFile(const char* const fileName, int fontSize) {
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
		return;
	}
	this->ft = ft;

	FT_Face face;
	if (FT_New_Face(ft, fileName, 0, &face))
	{
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
		return;
	}
	this->face = face;
	this->setFontSize(fontSize);
}

float TextRenderer::getLengthOfString(std::string& text, float scale) {
	float x = 0;
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = Characters[*c];
		x += ch.advance >> 6;
	}
	return x / getResolution().getResolutionWidth();
}

float TextRenderer::getHeightOfString(std::string& text, float scale) {
	float y = 0;
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = Characters[*c];
		float cmp = ch.size.y;
		if (cmp > y) {
			y = cmp;
		}
	}
	float ret = (y * scale) / getResolution().getResolutionHeight();
	return ret;
}

void TextRenderer::renderText(Shader& s, std::string text, float x, float y, int fontSize, glm::vec3 color) {
	renderText(s, text, x, y, (float)fontSize / this->fontSize, color);
}

void TextRenderer::renderText(Shader& s, std::string text, float x, float y, float scale, glm::vec3 color)
{
	if (x > 0 && x <= 1) {
		x = x / 2 + 0.5;
	}
	else {
		x = (1 - abs(x)) / 2;
	}
	if (y > 0 && y <= 1) {
		y = y / 2 + 0.5;
	}
	else {
		y = (1 - abs(y)) / 2;
	}
	x *= resolution->getResolutionWidth();
	y *= resolution->getResolutionHeight();
	ScreenResolution resolution = getResolution();
	s.bind();
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glm::mat4 projection = glm::ortho(0.0f, (float)resolution.getResolutionWidth(), 0.0f, (float)resolution.getResolutionHeight());
	glUniformMatrix4fv(glGetUniformLocation(s.getRendererId(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	// activate corresponding render state	
	s.setUniform3f("textColor", color[0], color[1], color[2]);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);

	// iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = Characters[*c];

		float xpos = x + ch.bearing.x * scale;
		float ypos = y - (ch.size.y - ch.bearing.y) * scale;

		float w = ch.size.x * scale;
		float h = ch.size.y * scale;
		// update VBO for each character
		float vertices[6][4] = {
			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos,     ypos,       0.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },

			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },
			{ xpos + w, ypos + h,   1.0f, 0.0f }
		};
		// render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.textureID);
		// update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
	s.unbind();
	glDisable(GL_BLEND);
	glDisable(GL_CULL_FACE);
}