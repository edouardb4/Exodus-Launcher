#pragma once

#include "Menu.h"

#include <algorithm>
#include <glut.h>

Menu::Menu(int window, std::shared_ptr<Renderer> renderer, std::shared_ptr<SystemInformation> systemInformation, std::shared_ptr<ScreenResolution> resolution) : 
	systemInformation(systemInformation), renderer(renderer), resolution(resolution), coordinates(Point2D<float>(0.0f, 0.0f)) {
	color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	buttons = std::make_shared<std::vector<std::shared_ptr<Button>>>();
	texture = nullptr;
	this->window = window;
	animations = std::make_shared<std::unordered_map<std::shared_ptr<Button>, std::shared_ptr<std::list<std::shared_ptr<MultiAnimationInvoker>>>>>();
	modifiers = std::make_shared<std::unordered_map<std::shared_ptr<Button>, std::shared_ptr<std::list<std::shared_ptr<ButtonModifier>>>>>();
}

int Menu::getWindow() {
	return window;
}

void Menu::setButtons(std::shared_ptr<std::vector<std::shared_ptr<Button>>> buttons) {
	this->buttons = buttons;
}

std::shared_ptr<std::vector<std::shared_ptr<Button>>> Menu::getButtons() {
	return buttons;
}

void Menu::setWidth(float width) {
	this->width = width;
}

void Menu::setHeight(float height) {
	this->height = height;
}

void Menu::bindTexture(std::shared_ptr<Texture> texture) {
	this->texture = texture;
}

bool Menu::isTextureBinded() {
	return texture != nullptr;
}

std::shared_ptr<Texture> Menu::getTexture() {
	return texture;
}

float Menu::getWidth() {
	return width;
}

float Menu::getHeight() {
	return height;
}

void Menu::setResolution(std::shared_ptr<ScreenResolution> resolution) {
	this->resolution = resolution;
}
std::list<std::shared_ptr<ButtonModifier>>* Menu::getModifiersFor(std::shared_ptr<Button> btn) {
	if (modifiers->count(btn) == 0) {
		return nullptr;
	}
	return modifiers->at(btn).get();
}

void Menu::pushModifier(std::shared_ptr<Button> btn, std::shared_ptr<ButtonModifier> modifier) {
	typedef std::list<std::shared_ptr<ButtonModifier>> type;
	bool contained = modifiers->count(btn) > 0;
	std::shared_ptr<type> list = contained ? modifiers->at(btn) : std::make_shared<type>();
	list->push_back(modifier);
	if (!contained) {
		modifiers->insert({ btn, list });
	}
}

void Menu::pushAnimation(std::shared_ptr<Button> btn, std::shared_ptr<MultiAnimationInvoker> animation) {
	typedef std::list<std::shared_ptr<MultiAnimationInvoker>> type;
	bool contained = animations->count(btn) > 0;
	std::shared_ptr<type> list = contained ? animations->at(btn) : std::make_shared<type>();
	list->push_back(animation);
	if (!contained) {
		animations->insert({ btn, list });
	}
}

std::list<std::shared_ptr<MultiAnimationInvoker>>* Menu::getAnimations(std::shared_ptr<Button> btn) {
	std::shared_ptr<std::list<std::shared_ptr<MultiAnimationInvoker>>> list = getAnimations()->at(btn);
}

std::unordered_map<std::shared_ptr<Button>, std::shared_ptr<std::list<std::shared_ptr<MultiAnimationInvoker>>>>* Menu::getAnimations() {
	return animations.get();
}

ScreenResolution* Menu::getResolution() {
	return resolution.get();
}

void Menu::processOutgoingAnimations() {
	for (auto pair : *getAnimations()) {
		Button* btn = pair.first.get();
		for(std::list<std::shared_ptr<MultiAnimationInvoker>>::reverse_iterator it = pair.second->rbegin(); it != pair.second->rend(); it++) {
			it->get()->process();
		}
	}
}

void Menu::drawButtons() {
	for (std::shared_ptr<Button> button : *buttons) {
		std::list<std::shared_ptr<ButtonModifier>>* modifiers = getModifiersFor(button);
		if (modifiers != nullptr) {
			for (std::shared_ptr<ButtonModifier> modifier : *modifiers) {
				modifier.get()->doEffect(button.get());
			}
		}
		processOutgoingAnimations();
		button->drawButton(systemInformation->getMouseX(), systemInformation->getMouseY());
	}
}

void Menu::addButton(std::shared_ptr<Button> button) {
	float width = button->getWidth();
	float height = button->getHeight();
	float x = button->getX();
	float y = button->getY();
	float modx = x;
	float mody = y;
	modx *= (this->getWidth() / resolution->getWidth());
	mody *= (this->getHeight() / resolution->getHeight());
	button->setX(this->getX() + this->getWidth() / 2 + modx);
	button->setY(this->getY() + this->getHeight() / 2 + mody);
	button->setWidth(width * (this->getWidth() / resolution->getWidth()));
	button->setHeight(height * (this->getHeight() / resolution->getHeight()));
	buttons->push_back(button);
}

bool Menu::isBound(float x, float y) {
	return this->coordinates.getX() <= x && this->coordinates.getY() <= y && this->coordinates.getX() + width >= x && this->coordinates.getY() + height >= y;
}

void Menu::setColor(glm::vec4 color) {
	this->color = color;
}

glm::vec4 Menu::getColor() {
	return color;
}

void Menu::setCoordinates(Point2D<float> coordinates) {
	this->coordinates = coordinates;
}

float Menu::getY() {
	return coordinates.getY();
}

float Menu::getX() {
	return coordinates.getX();
}

void Menu::draw(float mouseX, float mouseY) {
	if (isTextureBinded()) {
		renderer->renderImage(width, height, coordinates.getX(), coordinates.getY(), *texture);
	}
	renderer->drawRect(coordinates.getX(), coordinates.getY(), width, height, color);
	if (additionalDrawCall != nullptr) {
		additionalDrawCall();
	}
	drawButtons();
}

void Menu::processClicks() {
	for (std::shared_ptr<Button> button : *buttons) {
		if (button->isBound(systemInformation->getMouseX(), systemInformation->getMouseY())) {
			button->onClick();
		}
	}
}

bool Menu::isEnabled() {
	return enabled;
}

void Menu::setEnabled(bool enabled) {
	this->enabled = enabled;
}

void Menu::onTick(float mouseX, float mouseY) {
	draw(mouseX, mouseY);
	processClicks();
}