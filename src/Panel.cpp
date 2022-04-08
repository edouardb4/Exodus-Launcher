#include "Panel.h"
#include "ButtonComponent.h"
#include <type_traits>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

void Panel::setScrollbar(std::shared_ptr<Scrollbar> scrollbar) {
	this->scrollbar = scrollbar;
	float width = scrollbar->getWidth();
	float height = scrollbar->getHeight();
	float x = scrollbar->getCoordinates().getX();
	float y = scrollbar->getCoordinates().getY();
	float modx = x;
	float mody = y;
	modx *= (this->getWidth() / resolution->getWidth());
	mody *= (this->getHeight() / resolution->getHeight());
	scrollbar->setCoordinates(Point2D<float>(this->getX() + this->getWidth() / 2 + modx, this->getY() + this->getHeight() / 2 + mody));
	scrollbar->setWidth(width * (this->getWidth() / resolution->getWidth()));
	scrollbar->setHeight(height * (this->getHeight() / resolution->getHeight()));
}

Panel::Panel(std::string title, int window, std::shared_ptr<Renderer> renderer, std::shared_ptr<SystemInformation> systemInformation, std::shared_ptr<ScreenResolution> resolution) :
	Menu(window, renderer, systemInformation, resolution), title(title), scrollbar(nullptr) {
	this->components = std::make_shared<std::vector<std::shared_ptr<PanelComponent>>>();
	this->panelColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	this->scrollbarEnabled = false;
}

std::vector<std::shared_ptr<PanelComponent>>* Panel::getComponents() {
	return components.get();
}

void Panel::processClicks() {
	float shiftY = 0;
	if (scrollbarEnabled) {
		shiftY = -this->getScrollbar()->getPosition() * this->getDynamicHeight();
	}
	for (std::shared_ptr<Button> button : *buttons) {
		if (button->isBound(systemInformation->getMouseX(), shiftY + systemInformation->getMouseY())) {
			button->onClick();
		}
	}
}

Point2D<float> Panel::findLowestCoordinates() {
	Point2D<float> point = Point2D<float>(coordinates.getX(), coordinates.getY());
	std::vector<std::shared_ptr<PanelComponent>>* components = this->getComponents();
	for (std::vector<std::shared_ptr<PanelComponent>>::iterator it = components->begin(); it != components->end(); it++) {
		PanelComponent* component = it->get();
		if (component->getCoordinates().getY() < point.getY()) {
			point = Point2D<float>(component->getCoordinates().getX(), component->getCoordinates().getY());
		}
	}
	return point;
}

float Panel::getDynamicHeight() {
	return this->coordinates.getY() - findLowestCoordinates().getY();
}

void Panel::setTitle(std::string title) {
	this->title = title;
}

void Panel::setPanelHeight(float panelHeight) {
	this->panelHeight = panelHeight;
}

void Panel::setPanelMask(int panelMask) {
	this->panelMask = panelMask;
}

void Panel::setMask(int mask) {
	this->mask = mask;
}

void Panel::setTextRenderer(std::shared_ptr<TextRenderer> textRenderer) {
	this->textRenderer = textRenderer;
}

void Panel::setPanelColor(glm::vec4 panelColor) {
	this->panelColor = panelColor;
}

void Panel::setTextColor(glm::vec4 textColor) {
	this->textColor = textColor;
}

void Panel::pushComponent(std::shared_ptr<PanelComponent> component) {
	if (typeid(*component.get()) == typeid(ButtonComponent)) {
		Button* button = dynamic_cast<ButtonComponent*>(component.get())->getButton();
		addButton(std::shared_ptr<Button>(button));
	}
	float width = component->getWidth();
	float height = component->getHeight();
	float x = component->getCoordinates().getX();
	float y = component->getCoordinates().getY();
	float modx = x;
	float mody = y;
	modx *= (this->getWidth() / resolution->getWidth());
	mody *= (this->getHeight() / resolution->getHeight());
	component->setCoordinates(Point2D<float>(this->getX() + this->getWidth() / 2 + modx, this->getY() + this->getHeight() / 2 + mody));
	component->setWidth(width * (this->getWidth() / resolution->getWidth()));
	component->setHeight(height * (this->getHeight() / resolution->getHeight()));
	getComponents()->push_back(component);
}

void Panel::shiftAll(float x, float y) {
	for (std::shared_ptr<PanelComponent> component : *components.get()) {
		component->setCoordinates(Point2D<float>(component->getCoordinates().getX() + x, component->getCoordinates().getY() + y));
		if (typeid(*component.get()) == typeid(ButtonComponent)) {
			Button* button = static_cast<ButtonComponent*>(component.get())->getButton();
			button->setX(x + component->getCoordinates().getX());
			button->setY(y + component->getCoordinates().getY());
		}
	}
}

void Panel::draw(float mouseX, float mouseY) {
	if (additionalDrawCall != NULL) {
		additionalDrawCall();
	}
	if (isTextureBinded()) {
		renderer->renderImage(width, height - panelHeight, coordinates.getX(), coordinates.getY(), *texture);
	}
	renderer->drawCurvedRect(getX(), getY() + height - panelHeight, getWidth(), panelHeight, panelMask, panelColor);
	renderer->drawCurvedRect(getX(), getY(), getWidth(), height - panelHeight, mask, color);
	std::vector<std::shared_ptr<PanelComponent>>* components = this->getComponents();
	float shiftX = 0, shiftY = 0;
	if (scrollbarEnabled) {
		int gx, gy, gwidth, gheight;
		gx = (1.0f + this->getX()) / 2.0f * resolution->getResolutionWidth();
		gy = (1.0f + this->getY()) / 2.0f * resolution->getResolutionHeight();
		gwidth = this->getWidth() / 2.0f * resolution->getResolutionWidth();
		gheight = (this->getHeight() - panelHeight) / 2.0f * resolution->getResolutionHeight();
		glEnable(GL_SCISSOR_TEST);
		glScissor(gx, gy, gwidth, gheight);
		shiftX = 0;
		shiftY = scrollbar->getPosition() * getDynamicHeight();
		shiftAll(shiftX, shiftY);
	}
	for (std::vector<std::shared_ptr<PanelComponent>>::iterator it = components->begin(); it != components->end(); it++) {
		PanelComponent* component = it->get();
		component->draw(mouseX, mouseY);
	}
	if (scrollbarEnabled) {
		shiftAll(-shiftX, -shiftY);
		glDisable(GL_SCISSOR_TEST);
	}
	if (scrollbarEnabled) {
		scrollbar->draw(renderer.get(), findLowestCoordinates().getY(), height - panelHeight, coordinates.getY(), mouseX, mouseY);
	}
	renderer->drawText(textRenderer.get(), getX() + getWidth() / 2.0f - textRenderer->getLengthOfString(title, 1.0f), getY() + height - panelHeight / 2.0f -
		textRenderer->getHeightOfString(title, 1.0f) / 2.0f, title, textColor);
}