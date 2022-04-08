#include <ft2build.h>
#include FT_FREETYPE_H
#include <iostream>
#include <map>
#include "ScreenResolution.h"
#include "Shader.h"

#pragma once

struct Character {
	unsigned int textureID;  // ID handle of the glyph texture
	glm::ivec2   size;       // Size of glyph
	glm::ivec2   bearing;    // Offset from baseline to left/top of glyph
	unsigned int advance;    // Offset to advance to next glyph
};

class TextRenderer {
private:
	FT_Face face;
	FT_Library ft;
	const char* const fileName;
	std::shared_ptr<ScreenResolution> resolution;
	int fontSize;
	std::map<wchar_t, Character> Characters;
	float width;

public:
	TextRenderer(std::shared_ptr<ScreenResolution> const resolution, const char* const fileName, int fontSize);
	ScreenResolution& getResolution();
	void loadFile(const char* const fileName, int fontSize);
	void setFontSize(int fontSize) {
		this->fontSize = fontSize;
		FT_Set_Pixel_Sizes(face, 0, fontSize);
	}
	void loadGlyphs();
	void renderText(Shader& s, std::string text, float x, float y, float scale, glm::vec3 color);
	void renderText(Shader& s, std::string text, float x, float y, int fontSize, glm::vec3 color);
	float getLengthOfString(std::string& text, float scale);
	float getHeightOfString(std::string& text, float scale);
};
