#pragma once

#include <list>
#include "Button.h"
#include "SystemInformation.h"
#include "Texture.h"
#include "MultiAnimationInvoker.h"
#include "ButtonModifier.h"
#include "Point2D.h"

#include <list>

class Menu {
public:
	Menu(int window, std::shared_ptr<Renderer> renderer, std::shared_ptr<SystemInformation> systemInformation, std::shared_ptr<ScreenResolution> resolution);
	std::list<std::shared_ptr<ButtonModifier>>* getModifiersFor(std::shared_ptr<Button> button);
	std::unordered_map<std::shared_ptr<Button>, std::shared_ptr<std::list<std::shared_ptr<MultiAnimationInvoker>>>>* getAnimations();
	std::list<std::shared_ptr<MultiAnimationInvoker>>* getAnimations(std::shared_ptr<Button> btn);
	void pushAnimation(std::shared_ptr<Button>, std::shared_ptr<MultiAnimationInvoker> animation);
	void pushModifier(std::shared_ptr<Button>, std::shared_ptr<ButtonModifier> modifier);
	void setButtons(std::shared_ptr<std::vector<std::shared_ptr<Button>>> buttons);
	virtual std::shared_ptr<std::vector<std::shared_ptr<Button>>> getButtons();
	void setWidth(float width);
	void setHeight(float height);
	void setCoordinates(Point2D<float> coordinates);
	void bindTexture(std::shared_ptr<Texture> texture);
	std::shared_ptr<Texture> getTexture();
	bool isTextureBinded();
	ScreenResolution* getResolution();
	void setResolution(std::shared_ptr<ScreenResolution> resolution);
	bool isEnabled();
	void setEnabled(bool enabled);
	float getX();
	float getY();
	int getWindow();
	virtual void onTick(float mouseX, float mouseY);
	void addButton(std::shared_ptr<Button> button);
	virtual void processClicks();
	void drawButtons();
	void setColor(glm::vec4 color);
	bool isBound(float x, float y);
	glm::vec4 getColor();
	float getHeight();
	float getWidth();
	virtual void draw(float mouseX, float mouseY);
	void processOutgoingAnimations();
	template<typename Func>
	void setAdditionalDrawCall(Func func) {
		this->additionalDrawCall = func;
	}

protected:
	int window;
	float width, height;
	Point2D<float> coordinates;
	bool enabled;
	std::shared_ptr<SystemInformation> systemInformation;
	std::shared_ptr<std::vector<std::shared_ptr<Button>>> buttons;
	glm::vec4 color;
	std::shared_ptr<Texture> texture;
	std::shared_ptr<Renderer> renderer;
	std::shared_ptr<ScreenResolution> resolution;
	std::shared_ptr<std::unordered_map<std::shared_ptr<Button>, std::shared_ptr<std::list<std::shared_ptr<MultiAnimationInvoker>>>>> animations;
	std::shared_ptr<std::unordered_map<std::shared_ptr<Button>, std::shared_ptr<std::list<std::shared_ptr<ButtonModifier>>>>> modifiers;
	std::function<void()> additionalDrawCall;
};