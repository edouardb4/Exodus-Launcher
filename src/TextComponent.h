#pragma once

#include "PanelComponent.h"
#include "TextRenderer.h"
#include "Renderer.h"

#include <functional>

class TextComponent : public PanelComponent {
protected:
	std::string text;
	glm::vec4 textColor;
	std::shared_ptr<Renderer> renderer;
	std::shared_ptr<TextRenderer> textRenderer;
	std::shared_ptr<ScreenResolution> resolution;
	std::function<std::string()> stringFunction;
public:
	TextComponent(float x, float y, std::string text, std::shared_ptr<Renderer> renderer, std::shared_ptr<TextRenderer> textRenderer, std::shared_ptr<ScreenResolution> resolution);
	glm::vec4 getTextColor();
	virtual void draw(float mouseX, float mouseY);
	TextRenderer* getTextRenderer();
	void setTextColor(glm::vec4 textColor);
	void setTextRenderer(std::shared_ptr<TextRenderer> textRenderer);
	template<typename Func>
	void setStringFunction(Func stringFunction) {
		this->stringFunction = stringFunction;
	}
};