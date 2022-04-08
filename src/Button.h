#pragma once
#include "ButtonType.h"
#include "Renderer.h"
#include "CenterType.h"
#include <functional>

class Button {
public:
	std::shared_ptr<Renderer> renderer;
	std::shared_ptr<Texture> bindedTexture;
	std::shared_ptr<TextRenderer> textRenderer;
	glm::vec4 colorScheme;
	glm::vec4 borderColorScheme;
	glm::vec4 hoverColorScheme;
	glm::vec4 textColorScheme;
	ButtonType buttonType;
	CenterType centerType;
	std::function<void()> additionalDrawCall;
	std::function<void()> clickFunction;
	std::function<void(bool)> onHoverFunction;
	bool borderEnabled;
	bool changeCursor;
	float x, y;
	float width, height;
	float borderWidth;
	std::string textToRender;
	std::string internalName;

public:
	Button(std::shared_ptr<Renderer> renderer, float x, float y, float width, float height);
	Texture* getBindedTexture();
	Renderer* getRenderer();
	CenterType getCenterType();
	ButtonType getButtonType();
	glm::vec4 getColorScheme();
	glm::vec4 getBorderColorScheme();
	glm::vec4 getTextColorScheme();
	glm::vec4 getHoverColorScheme();
	bool isBound(float x, float y);
	bool isBorderEnabled();
	bool isTextureBinded();
	float getHeight();
	float getWidth();
	void setChangeCursor(bool changeCursor);
	float getY();
	float getX();
	float getBorderWidth();
	virtual void drawButton(float mouseX, float mouseY);
	virtual void drawDefaultButton(float mouseX, float mouseY);
	void setInternalName(std::string internalName);
	std::string getInternalName();
	void setTextRenderer(std::shared_ptr<TextRenderer> textRenderer);
	void onClick();
	void setBorderEnabled(bool borderEnabled);
	void setWidth(float width);
	void setCenterType(CenterType type);
	void bindTexture(std::shared_ptr<Texture> bindedTexture);
	void setHeight(float height);
	void setBorderWidth(float borderWidth);
	void setHoverColorScheme(glm::vec4 hoverColorScheme);
	void setColorScheme(glm::vec4 colorScheme);
	void setBorderColorScheme(glm::vec4 borderColorScheme);
	void setTextColorScheme(glm::vec4 textColorScheme);
	void setTextToRender(std::string textToRender);
	void setButtonType(ButtonType buttonType);
	void setX(float x);
	void setY(float y);
	template<typename Func>
	void setOnClick(Func clickFunction) {
		this->clickFunction = clickFunction;
	}
	template<typename Func>
	void setAdditionalDrawCall(Func additionalDrawCall) {
		this->additionalDrawCall = additionalDrawCall;
	}
	template<typename Func>
	void setOnHover(Func onHoverFunction) {
		this->onHoverFunction = onHoverFunction;
	}
};