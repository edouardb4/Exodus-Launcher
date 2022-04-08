#pragma once

class SystemInformation {
private:
	float mouseX;
	float mouseY;
	float mouseClickX;
	float mouseClickY;
	bool pressed;
public:
	float getMouseX();
	float getMouseY();
	void setMouseX(float mouseX);
	void setMouseY(float mouseY);
	unsigned long long getTotalRAMBytes();
};