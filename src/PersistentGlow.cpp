#include "PersistentGlow.h"
#include "Renderer.h"

#include <GL/glew.h>
#include <GL/GL.h>
#include <GLFW/glfw3.h>

PersistentGlow::PersistentGlow(std::shared_ptr<ScreenResolution> screenResolution, glm::vec3 color, float intensity, float scaleWidth, float scaleHeight) : color(color), intensity(intensity),
scaleWidth(scaleWidth), scaleHeight(scaleHeight), screenResolution(screenResolution) {
}

void PersistentGlow::doEffect(Button* button) {
	float height = this->scaleHeight * button->getHeight();
	float width = this->scaleWidth * button->getWidth();
	int resolutionHeight = screenResolution->getResolutionHeight();
	int resolutionWidth = screenResolution->getResolutionWidth();
	int x = button->getX() * resolutionWidth;
	int y = button->getY() * resolutionHeight;
	glm::vec4 colorScheme = glm::vec4(color[0], color[1], color[2], intensity);
	button->getRenderer()->drawCurvedRect(button->getX() - width / 2, button->getY() - height / 2, width + button->getWidth(), height + button->getHeight(), 15, colorScheme);
}