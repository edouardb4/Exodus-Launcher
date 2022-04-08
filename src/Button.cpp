#include "Button.h"
#include "CurveMask.h"

Button::Button(std::shared_ptr<Renderer> renderer, float x, float y, float width, float height) : renderer(renderer), x(x), y(y), width(width), height(height) {
	textToRender = "";
	buttonType = ButtonType::RECT;
	textColorScheme = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	colorScheme = glm::vec4(0.5f, 1.0f, 1.0f, 1.0f);
	hoverColorScheme = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	borderWidth = 1.0f;
	textRenderer = renderer->getTextRenderer();
}

Renderer* Button::getRenderer() {
	return renderer.get();
}

float Button::getHeight() {
	return this->height;
}

float Button::getWidth() {
	return this->width;
}

void Button::setButtonType(ButtonType buttonType) {
	this->buttonType = buttonType;
}

ButtonType Button::getButtonType() {
	return buttonType;
}

void Button::setColorScheme(glm::vec4 colorScheme) {
	this->colorScheme = colorScheme;
}

void Button::setBorderColorScheme(glm::vec4 borderColorScheme) {
	this->borderColorScheme = borderColorScheme;
}

void Button::setTextColorScheme(glm::vec4 textColorScheme) {
	this->textColorScheme = textColorScheme;
}

glm::vec4 Button::getColorScheme() {
	return colorScheme;
}

glm::vec4 Button::getBorderColorScheme() {
	return borderColorScheme;
}

glm::vec4 Button::getTextColorScheme() {
	return textColorScheme;
}

void Button::setHeight(float height) {
	this->height = height;
}

void Button::setWidth(float width) {
	this->width = width;
}

void Button::setY(float y) {
	this->y = y;
}

void Button::setX(float x) {
	this->x = x;
}

float Button::getY() {
	return y;
}

void Button::onClick() {
	if (this->clickFunction != nullptr) {
		this->clickFunction();
	}
}

bool Button::isBound(float x, float y) {
	return this->x <= x && this->y <= y && this->x + width >= x && this->y + height >= y;
}

glm::vec4 Button::getHoverColorScheme() {
	return hoverColorScheme;
}

void Button::setHoverColorScheme(glm::vec4 hoverColorScheme) {
	this->hoverColorScheme = hoverColorScheme;
}

void Button::setCenterType(CenterType centerType) {
	this->centerType = centerType;
}

CenterType Button::getCenterType() {
	return centerType;
}

float Button::getX() {
	return x;
}

void Button::setBorderEnabled(bool borderEnabled) {
	this->borderEnabled = borderEnabled;
}

Texture* Button::getBindedTexture() {
	return bindedTexture.get();
}

void Button::bindTexture(std::shared_ptr<Texture> bindedTexture) {
	this->bindedTexture = bindedTexture;
}

void Button::setBorderWidth(float borderWidth) {
	this->borderWidth = borderWidth;
}

float Button::getBorderWidth() {
	return borderWidth;
}

bool Button::isTextureBinded() {
	return bindedTexture != nullptr;
}

bool Button::isBorderEnabled() {
	return borderEnabled;
}

void Button::setTextToRender(std::string textToRender) {
	this->textToRender = textToRender;
}

void Button::drawButton(float mouseX, float mouseY) {
	drawDefaultButton(mouseX, mouseY);
	if (isTextureBinded()) {
		renderer->renderImage(width, height, x, y, *bindedTexture.get());
	}
}

void Button::setTextRenderer(std::shared_ptr<TextRenderer> textRenderer) {
	this->textRenderer = textRenderer;
}

void Button::setInternalName(std::string internalName) {
	this->internalName = internalName;
}

void Button::setChangeCursor(bool changeCursor) {
	this->changeCursor = changeCursor;
}

std::string Button::getInternalName() {
	return internalName;
}

void Button::drawDefaultButton(float mouseX, float mouseY) {
	glm::vec4 colorToUse = isBound(mouseX, mouseY) ? hoverColorScheme : colorScheme;
	std::list<CurveType> list;
	list.push_back(CurveType::TOP_LEFT);
	list.push_back(CurveType::TOP_RIGHT);
	ButtonType type = getButtonType();
	if (type == ButtonType::TRIANGLE) {
		renderer->drawTriangle(x + width, y + height, width * 2, height * 2, colorScheme);
	}
	else {
		int mask = type == ButtonType::ROUND_RECT ? CurveMask::getAllBits() : type == ButtonType::TOP_ROUND_RECT ? CurveMask::combineMask(list) : type == ButtonType::RECT ? 0 : 0;
		renderer->drawCurvedRect(x, y, width, height, mask, colorToUse);
		if (borderEnabled) {
			renderer->drawHollowCurvedRect(x, y, width, height, mask, borderWidth, borderColorScheme);
		}
	}
	if (textToRender.length() > 0) {
		TextRenderer* textRenderer = this->textRenderer.get();
		float xOffset = (centerType == CenterType::BOTH || centerType == CenterType::X_CENTERED ? width / 2 - textRenderer->getLengthOfString(textToRender, 1.0f) : 0);
		float yOffset = (centerType == CenterType::BOTH || centerType == CenterType::Y_CENTERED ? height / 2 - textRenderer->getHeightOfString(textToRender, 1.0f) : 0);
		renderer->drawText(textRenderer, x + xOffset, y + yOffset, textToRender, textColorScheme);
	}
	if (additionalDrawCall != nullptr) {
		this->additionalDrawCall();
	}
}