#pragma once
#ifndef PANEL_H
#define PANEL_H

#include <vector>
#include "Menu.h"
#include "Point2D.h"
#include "PanelComponent.h"
#include "Scrollbar.h"

class Panel : public Menu {
public:
	Panel(std::string title, int window, std::shared_ptr<Renderer> renderer, std::shared_ptr<SystemInformation> systemInformation, std::shared_ptr<ScreenResolution> resolution);
	virtual void draw(float mouseX, float mouseY);
	virtual void processClicks();
	void setTitle(std::string title);
	void setTextRenderer(std::shared_ptr<TextRenderer> textRenderer);
	std::vector<std::shared_ptr<PanelComponent>>* getComponents();
	void shiftAll(float x, float y);
	void pushComponent(std::shared_ptr<PanelComponent> component);
	void setPanelHeight(float panelHeight);
	void setPanelColor(glm::vec4 panelColor);
	void setTextColor(glm::vec4 textColor);
	void setPanelMask(int mask);
	void setMask(int mask);
	void setScrollbar(std::shared_ptr<Scrollbar> scrollbar);
	void setScrollbarEnabled(bool scrollbarEnabled) { this->scrollbarEnabled = scrollbarEnabled; }
	bool isScrollbarEnabled() { return scrollbarEnabled; }
	float getDynamicHeight();
	Scrollbar* getScrollbar() { return scrollbar.get(); }
	Point2D<float> findLowestCoordinates();
protected:
	float panelHeight;
	int panelMask, mask;
	bool scrollbarEnabled;
	std::shared_ptr<Scrollbar> scrollbar;
	glm::vec4 textColor, panelColor;
	std::string title;
	std::shared_ptr<TextRenderer> textRenderer;
	std::shared_ptr<std::vector<std::shared_ptr<PanelComponent>>> components;
};

#endif