#include "TextComponent.h"

TextComponent::TextComponent(float x, float y, std::string text, std::shared_ptr<Renderer> renderer, std::shared_ptr<TextRenderer> textRenderer, std::shared_ptr<ScreenResolution> resolution) : PanelComponent(Point2D<float>(x, y)) {
	this->textRenderer = textRenderer;
	this->renderer = renderer;
	this->resolution = resolution;
	this->text = text;
}

glm::vec4 TextComponent::getTextColor() {
	return textColor;
}
TextRenderer* TextComponent::getTextRenderer() {
	return textRenderer.get();
}

void TextComponent::setTextColor(glm::vec4 textColor) {
	this->textColor = textColor;
}

void TextComponent::setTextRenderer(std::shared_ptr<TextRenderer> textRenderer) {
	this->textRenderer = textRenderer;
}

void TextComponent::draw(float mouseX, float mouseY) {
	std::string text = (stringFunction != nullptr ? stringFunction() : this->text);
	renderer->drawText(textRenderer.get(), coordinates.getX(), coordinates.getY(), text, textColor);
}