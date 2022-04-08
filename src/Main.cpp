#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <freeglut.h>
#include <map>
#include <utility>
#include <functional>
#include <random>
#include <timeapi.h>
#include <thread>
#include <future> 
#include <typeinfo>
#include <tchar.h>
#include <stdexcept>
#include <stdio.h>
#include <windows.h>
#include <filesystem>
#include <Windows.h>
#include <cstdlib>
#include <windows.h>
#include <stdio.h>
#include <fstream>
#include <windows.h>
#include <shellapi.h>
#include "ShaderRegistry.h"
#include "Shader.h"
#include "ScreenResolution.h"
#include "Renderer.h"
#include "TextRendererRegistry.h"
#include "SystemInformation.h"
#include "MenuManager.h"
#include "MathUtils.h"
#include "ExitButton.h"
#include "FadeInOutAnimation.h"
#include "PersistentGlow.h"
#include "SwappableComponent.h"
#include "LauncherSettings.h"
#include "SliderButton.h"
#include "Panel.h"
#include "ButtonComponent.h"
#include "CurveMask.h"
#include "TextComponent.h"
#include "AuthRequest.h"
#include "DownloadContent.h"
#include "Scrollbar.h"
#include "DropdownButton.h"
#include "AccountManager.h"
#include "FreedrawComponent.h"
#include "ExodusLauncher.h"
#include "DemonicLauncher.h"
#include "CosmicLauncher.h"
std::shared_ptr<TextRendererRegistry> textRendererRegistry;
std::shared_ptr<ShaderRegistry> shaderRegistry;
std::shared_ptr<ScreenResolution> screenResolution;
std::shared_ptr<Renderer> renderer;
std::shared_ptr<SystemInformation> systemInformation;
std::shared_ptr<MenuManager> menuManager;
std::shared_ptr<SwappableComponent<std::string>> swappableComponent;
std::shared_ptr<Minecraft::AccountManager> accountManager;
std::string playingServer = "";
LauncherSettings::Profile* profile;
int window;
bool hide = false;
int hideInt = 0;

using namespace LauncherSettings;


template<typename Base, typename T>
inline bool instanceof(const T* t) {
	return typeid(*t) == typeid(Base);
}

void hideConsole()
{
	::ShowWindow(::GetConsoleWindow(), SW_HIDE);
}

void showConsole()
{
	::ShowWindow(::GetConsoleWindow(), SW_SHOW);
}

void saveSettings() {
	LauncherSettings::saveToFile(profile, "profilesettings.json");
}

void process(std::string str) {
	std::this_thread::sleep_for(std::chrono::seconds(1));
	std::system(str.c_str());
	glutShowWindow();
}

LSTATUS MyGetEnvironmentVariable(LPCWSTR name, std::wstring& out) {
	DWORD res, size = MAX_PATH;
	out = std::wstring();
	out.resize(size);
	while ((res = GetEnvironmentVariable(name, &out[0], size)) == size) {
		out.resize(size += MAX_PATH);
	}
	if (res == 0)
		return GetLastError();
	else {
		out.resize(size - MAX_PATH + res);
		return ERROR_SUCCESS;
	}
}

bool MyCreateProcess(const std::wstring& command, const std::wstring& workdir) {
std::wstring writable_command = command;
STARTUPINFO si;
PROCESS_INFORMATION pi;
si.cb = sizeof(si);
ZeroMemory(&si, sizeof(si));
ZeroMemory(&pi, sizeof(pi));

if (workdir.empty()) {
	return CreateProcess(NULL, &writable_command[0], NULL, NULL, false,
		NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi);
}
else {
	return CreateProcess(NULL, &writable_command[0], NULL, NULL, false,
		NORMAL_PRIORITY_CLASS, NULL, workdir.c_str(), &si,
		&pi);
}
}

std::wstring wstring_from_bytes(std::string const& str)
{
	size_t requiredSize = 0;
	std::wstring answer;
	wchar_t* pWTempString = NULL;

	/*
	* Call the conversion function without the output buffer to get the required size
	*  - Add one to leave room for the NULL terminator
	*/
	requiredSize = mbstowcs(NULL, str.c_str(), 0) + 1;

	/* Allocate the output string (Add one to leave room for the NULL terminator) */
	pWTempString = (wchar_t*)malloc(requiredSize * sizeof(wchar_t));
	if (pWTempString == NULL)
	{
		printf("Memory allocation failure.\n");
	}
	else
	{
		// Call the conversion function with the output buffer
		size_t size = mbstowcs(pWTempString, str.c_str(), requiredSize);
		if (size == (size_t)(-1))
		{
			printf("Couldn't convert string\n");
		}
		else
		{
			answer = pWTempString;
		}
	}


	if (pWTempString != NULL)
	{
		delete[] pWTempString;
	}

	return answer;
}


void launch() {
	std::string wd = std::filesystem::current_path().generic_string();
	std::string slash = "\"";
	std::string active = profile->getActiveClient();
	if (active == "Exodus Client") {
		ExodusLauncher launcher;
		launcher.downloadContent();
	}
	else if (active == "Demonic Client") {
		DemonicLauncher launcher;
		launcher.downloadContent();
	}
	else if (active == "Cosmic Client") {
		CosmicLauncher launcher;
		launcher.downloadContent();
	}
	active = active.substr(0, active.find(" ")) + active.substr(active.find(" ") + 1);
	int mb = profile->getDedicatedBytes() / (1024LL * 1024LL);
	if (mb < 1024 * 2) {
		mb = 1024 * 2;
	}
	std::string str = slash + wd + "/jre_64/bin/javaw.exe" + slash + " -Djava.library.path=" + slash + wd + "/1.8/bin-1.8" + slash + " -Xms2048M -Xmx4096M -XX:MaxDirectMemorySize=4096M -XX:+DisableAttachMechanism -Xshare:off -Xmn128M -Djava.net.preferIPv4Stack=true -Dcom.cosmicpvp.librarypath=true -jar " + slash + wd + "/1.8/" + active + ".jar" + slash + " --version 1.8 --assetsDir " + slash + wd + "/assets_18" + slash + " --assetIndex 1.8 --accessToken  --userProperties {} --width 854 --height 480 --novid";
	if (playingServer.length() > 0) {
		str += " --server " + playingServer;
		playingServer = "";
	}
	auto path = std::filesystem::temp_directory_path()
		.parent_path()
		.parent_path()
		.parent_path();
	path /= "Roaming";
	std::cout << path.string() << std::endl;
	MyCreateProcess(wstring_from_bytes(str), wstring_from_bytes(path.string() + "\\.minecraft"));
}

void doComponentLogic() {
	if (hide && hideInt++ == 10) {
		hide = false;
		hideInt = 0;
		launch();
		glutShowWindow();
	}
	std::string active = swappableComponent.get()->getActiveComponent();
	for (std::string comp : *(swappableComponent.get()->getComponentList())) {
		menuManager.get()->getMenu(comp.c_str()).get()->setEnabled(false);
	}
	menuManager.get()->getMenu("playpanel")->setEnabled(active == "mainmenu");
	menuManager.get()->getMenu("other_mainmenu")->setEnabled(active == "mainmenu");
	menuManager.get()->getMenu(active).get()->setEnabled(true);
	for (std::string comp : *swappableComponent.get()->getComponentList()) {
		std::shared_ptr<Menu> menu = menuManager->getMenu("headermenu");
		std::shared_ptr<std::vector<std::shared_ptr<Button>>> buttons = menu->getButtons();
		std::vector<std::shared_ptr<Button>>::iterator it = buttons->begin();
		for (std::vector<std::shared_ptr<Button>>::iterator it = buttons->begin(); it != buttons->end(); it++) {
			Button* button = it->get();
			if (button->getInternalName().length() == 0) {
				continue;
			}
			button->setBorderEnabled(false);
			if (strcmp(button->getInternalName().c_str(), swappableComponent->getActiveComponent().c_str())) {
				button->setTextColorScheme(glm::vec4(169.0f / 255.0f, 169.0f / 255.0f, 169.0f / 255.0f, 1.0f));
				continue;
			}
			if (button->textToRender == "STORE" || button->textToRender == "ABOUT" || button->textToRender == "DISCORD") {
				continue;
			}
			button->setTextColorScheme(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
			button->setColorScheme(glm::vec4(50.0f / 255.0f, 50.0f / 255.0f, 50.0f / 255.0f, 0.0f));
		}
	}
}

void renderMenus() {
	for (auto pair : *(menuManager.get()->getMenus())) {
		Menu* menu = pair.second.get();
		if (!menu->isEnabled()) {
			continue;
		}
		menu->draw(systemInformation->getMouseX(), systemInformation->getMouseY());
	}
	float min = MathUtils::min(1.0f, 2.0f);
}

void render() {
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	renderMenus();
	glutSwapBuffers();
}

void update(int argc) {
	doComponentLogic();
	render();
	glutTimerFunc(10, update, 0);
}

void onMouseWheel(int button, int dir, int x, int y) {
	for (auto pair : *menuManager->getMenus()) {
		Menu* menu = pair.second.get();
		if (!menu->isEnabled() || !instanceof<Panel>(menu)) {
			continue;
		}
		if (!menu->isBound(systemInformation->getMouseX(), systemInformation->getMouseY())) {
			continue;
		}
		Panel* panel = static_cast<Panel*>(menu);
		if (!panel->isScrollbarEnabled()) {
			continue;
		}
		float speed = (1.0f / (panel->getDynamicHeight() / panel->getHeight())) * 0.05f;
		Scrollbar* scrollbar = panel->getScrollbar();
		scrollbar->setPosition(MathUtils::max<float>(0.0f, MathUtils::min<float>(1.0f, scrollbar->getPosition() + -dir * speed)));
	}
}

void onMouseMove(int x, int y) {
	systemInformation->setMouseX((float) x / screenResolution->getResolutionWidth() * 2 - 1);
	systemInformation->setMouseY(1 - (float) y / screenResolution->getResolutionHeight() * 2);
	for (auto pair : *menuManager->getMenus()) {
		Menu* menu = pair.second.get();
		if (!menu->isEnabled()) {
			continue;
		}
		std::vector<std::shared_ptr<Button>>* vec = menu->getButtons().get();
		for (std::vector<std::shared_ptr<Button>>::iterator it = vec->begin(); it != vec->end(); it++) {
			Button* button = it->get();
			if (instanceof<SliderButton>(button)) {
				SliderButton& slider = *dynamic_cast<SliderButton*>(button);
				if (!slider.isMouseDown()) {
					continue;
				}
				slider.setSliderPosition(MathUtils::max(0.0f, MathUtils::min(1.0f, (systemInformation->getMouseX() - button->getX()) / button->getWidth())));
			}
		}
	}
}

void onMouseClick(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON || button == GLUT_RIGHT_BUTTON) {
		if (state == GLUT_UP) {
			for (auto pair : *menuManager->getMenus()) {
				Menu* menu = pair.second.get();
				if (!menu->isEnabled()) {
					continue;
				}
				std::vector<std::shared_ptr<Button>>* vec = menu->getButtons().get();
				for (std::vector<std::shared_ptr<Button>>::iterator it = vec->begin(); it != vec->end(); it++) {
					Button* button = it->get();
					if (instanceof<SliderButton>(button)) {
						SliderButton& slider = *dynamic_cast<SliderButton*>(button);
						if (!slider.isMouseDown()) {
							continue;
						}
						slider.setSliderPosition(MathUtils::max(0.0f, MathUtils::min(1.0f, (systemInformation->getMouseX() - button->getX()) / button->getWidth())));
						if (slider.getReleaseFunction() != nullptr) {
							slider.getReleaseFunction()();
						}
					}
				}
				menu->processClicks();
			}
		}
		else if (state == GLUT_DOWN) {
			for (auto pair : *menuManager->getMenus()) {
				Menu* menu = pair.second.get();
				if (!menu->isEnabled()) {
					continue;
				}
				std::vector<std::shared_ptr<Button>>* vec = menu->getButtons().get();
				for (std::vector<std::shared_ptr<Button>>::iterator it = vec->begin(); it != vec->end(); it++) {
					Button* button = it->get();
					float shiftY = 0;
					if (typeid(*menu) == typeid(Panel)) {
						Panel* panel = static_cast<Panel*>(menu);
						if (panel->isScrollbarEnabled()) {
							shiftY = -panel->getScrollbar()->getPosition() * panel->getDynamicHeight();
						}
					}
					if (!button->isBound(systemInformation->getMouseX(), shiftY + systemInformation->getMouseY())) {
						continue;
					}
					if (instanceof<SliderButton>(button)) {
						SliderButton& slider = *dynamic_cast<SliderButton*>(button);
						if (slider.getHoldFunction() != nullptr) {
							slider.getHoldFunction()();
						}
					}
				}
			}
		}
	}
}

void onExitClick() {
	saveSettings();
	if (std::ifstream("/old-Launcher.exe").good()) {
		std::remove("/old-Launcher.exe");
	}
	glutLeaveMainLoop();
	glutDestroyWindow(window);
}

void registerHeaderPanel() {
	std::string path = "res/pictures/background_color.png";
	std::shared_ptr<Menu> menu = std::make_shared<Menu>(window, renderer, systemInformation, screenResolution);
	menu->setEnabled(true);
	menu->setHeight(0.2f);
	menu->setWidth(2.0f);
	menu->setCoordinates(Point2D<float>(-1.0f, 0.8f));
	menu->setColor(glm::vec4(115.0f / 255.0f, 180.0f / 255.0f, 255.0f / 255.0f, 0.0f));
	menu->bindTexture(std::make_shared<Texture>("res/pictures/color.png"));

	std::shared_ptr<ExitButton> exitButton = std::make_shared<ExitButton>(renderer, 0.95f, 0.25f, 0.05f, 0.9f);
	exitButton->setButtonType(ButtonType::TRIANGLE);
	exitButton->setBorderColorScheme(glm::vec4(134.0f / 255.0f, 134.0f / 255.0f, 134.0f / 255.0f, 0.5f));
	exitButton->setColorScheme(glm::vec4(48.0f / 255.0f, 48.0f / 255.0f, 48.0f / 255.0f, 0.5f));
	exitButton->setDrawColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	exitButton->setHoverColorScheme(glm::vec4(255.0f / 255.0f, 48.0f / 255.0f, 48.0f / 255.0f, 0.75f));
	exitButton->setBorderEnabled(true);
	exitButton->setOnClick(&onExitClick);

	std::string clientText = "Exodus Client";
	path = "res/pictures/exohead.png";
	std::shared_ptr<TextRenderer> textRenderer = textRendererRegistry->lookup("notoserif");
	std::shared_ptr<Button> logoButton = std::make_shared<Button>(renderer, -0.9f, -0.55f, 0.08f, 1.15f);
	logoButton->setColorScheme(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
	logoButton->setHoverColorScheme(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
	logoButton->setCenterType(CenterType::BOTH);
	logoButton->bindTexture(std::make_shared<Texture>(path));
	logoButton->setTextRenderer(textRenderer);


	float offset = textRenderer->getLengthOfString(clientText, 1.0f) * 1.5 + 0.25f;

	menu->addButton(exitButton);
	menu->addButton(logoButton);

	auto logoDrawCall = [menu, offset]() {
		//renderer->drawLine(menu->getX() + offset, menu->getY(), menu->getX() + offset, menu->getY() + 1.0f, 1.0f, glm::vec4(1.0f, 1.0f, 1.0f, 0.25f));
	};

	menu->setAdditionalDrawCall(logoDrawCall);

	std::list<std::pair<std::string, std::string>> list;
	list.push_back({ "mainmenu", "HOME" });
	list.push_back({ "settingsmenu", "SETTINGS" });
	list.push_back({ "settingsmenu", "STORE" });
	list.push_back({ "settingsmenu", "ABOUT" });
	float height = 0.75f;
	float width = 0.2f;
	float padding = width + 0.05f;
	float y = 0.0f - height / 2;
	float offsetx = 0.0f - (list.size() - 1) / 2.0f * padding;
	for (std::pair<std::string, std::string> pair : list) {
		std::shared_ptr<Button> button = std::make_shared<Button>(renderer, offsetx, y, width, height);
		button->setTextToRender(pair.second);
		button->setColorScheme(glm::vec4(50.0f / 255.0f, 50.0f / 255.0f, 50.0f / 255.0f, 0.0f));
		button->setBorderEnabled(true);
		button->setBorderColorScheme(glm::vec4(1.0f, 1.0f, 1.0f, 0.25f));
		button->setHoverColorScheme(glm::vec4(86.0f / 255.0f, 86.0f / 255.0f, 86.0f / 255.0f, 0.1f));
		button->setButtonType(ButtonType::RECT);
		button->setCenterType(CenterType::BOTH);
		button->setInternalName(pair.first);
		button->setTextRenderer(textRendererRegistry->lookup("walkthemoon"));
		auto onClick = [pair]() {
			if (pair.second == "STORE") {
				ShellExecute(0, 0, L"http://buy.exoduspvp.org", 0, 0, SW_SHOW);
				return;
			}
			else if (pair.second == "DISCORD") {
				ShellExecute(0, 0, L"http://discord.gg/exoduspvp", 0, 0, SW_SHOW);
				return;
			}
			else if (pair.second == "ABOUT") {
				return;
			}
			std::vector<std::string>* list = swappableComponent.get()->getComponentList();
			auto it = std::find(list->begin(), list->end(), pair.first);
			swappableComponent->setIndex(it - list->begin());
		};
		button->setOnClick(onClick);
		menu->addButton(button);
		offsetx += padding;
		swappableComponent.get()->pushComponent(pair.first);
	}

	menuManager->addMenu("headermenu", menu);
}

void registerFooterPanel() {
	std::shared_ptr<Menu> menu = std::make_shared<Menu>(window, renderer, systemInformation, screenResolution);
	menu->setEnabled(true);
	menu->setHeight(0.1f);
	menu->setWidth(2.0f);
	menu->setCoordinates(Point2D<float>(-1.0f, -1.0f));
	menu->setColor(glm::vec4(22.0f / 255.0f, 22.0f / 255.0f, 22.0f / 255.0f, 0.25f));

	menuManager->addMenu("footermenu", menu);
	TextRenderer* textRenderer = textRendererRegistry->lookup("titillium_16").get();
	auto additionalDrawCall = [textRenderer, menu]() {
		std::string right = "Not Affiliated With Mojang, AB.";
		std::string left = "ExodusPvP (2019-2021)";
		renderer->drawText(textRenderer, menu->getX() + menu->getWidth() - 0.05f - textRenderer->getLengthOfString(right, 1.0f) * 2, menu->getY() - textRenderer->getHeightOfString(right, 1.0f) / 2 + menu->getHeight() / 2, right, glm::vec4(0.75f, 0.75f, 0.75f, 1.0f));
		renderer->drawText(textRenderer, menu->getX() + 0.05, menu->getY() - textRenderer->getHeightOfString(left, 1.0f) / 2 + menu->getHeight() / 2, left, glm::vec4(0.75f, 0.75f, 0.75f, 1.0f));
	};

	auto onClick = []() {
		ShellExecute(0, 0, L"http://discord.gg/exoduspvp", 0, 0, SW_SHOW);
	};

	std::shared_ptr<Button> button = std::make_shared<Button>(renderer, -0.1, -0.725f, 0.085f, 1.5f);
	button->setButtonType(ButtonType::RECT);
	button->setBorderColorScheme(glm::vec4(1.0f, 1.0f, 1.0f, 0.0f));
	button->bindTexture(std::make_shared<Texture>("res/pictures/discord_icon.png"));
	button->setColorScheme(glm::vec4(1.0f, 1.0f, 1.0f, 0.0f));
	button->setHoverColorScheme(glm::vec4(1.0f, 1.0f, 1.0f, 0.0f));
	button->setOnClick(onClick);

	auto onStoreClick = []() {
		ShellExecute(0, 0, L"http://buy.exoduspvp.org", 0, 0, SW_SHOW);
	};

	std::shared_ptr<Button> storeButton = std::make_shared<Button>(renderer, -0.175f, -0.55f, 0.055f, 1.1f);
	storeButton->setButtonType(ButtonType::RECT);
	storeButton->setBorderColorScheme(glm::vec4(1.0f, 1.0f, 1.0f, 0.0f));
	storeButton->bindTexture(std::make_shared<Texture>("res/pictures/store.png"));
	storeButton->setColorScheme(glm::vec4(1.0f, 1.0f, 1.0f, 0.0f));
	storeButton->setHoverColorScheme(glm::vec4(1.0f, 1.0f, 1.0f, 0.0f));
	storeButton->setOnClick(onStoreClick);

	std::shared_ptr<Button> youtubeButton = std::make_shared<Button>(renderer, 0.01f, -0.55f, 0.055f, 1.1f);
	youtubeButton->setButtonType(ButtonType::RECT);
	youtubeButton->setBorderColorScheme(glm::vec4(1.0f, 1.0f, 1.0f, 0.0f));
	youtubeButton->bindTexture(std::make_shared<Texture>("res/pictures/youtube.png"));
	youtubeButton->setColorScheme(glm::vec4(1.0f, 1.0f, 1.0f, 0.0f));
	youtubeButton->setHoverColorScheme(glm::vec4(1.0f, 1.0f, 1.0f, 0.0f));

	menu->addButton(button);
	menu->addButton(storeButton);
	menu->addButton(youtubeButton);
	menu->setAdditionalDrawCall(additionalDrawCall);
}

void registerBottomPanel() {
	std::shared_ptr<Menu> panel = std::make_shared<Menu>(window, renderer, systemInformation, screenResolution);
	panel->setEnabled(true);
	panel->setHeight(0.1f);
	panel->setWidth(2.0f);
	panel->setCoordinates(Point2D<float>(-1.0f, -0.05f));
	panel->setColor(glm::vec4(115.0f / 255.0f, 180.0f / 255.0f, 255.0f / 255.0f, 0.0f));
	//panel->bindTexture(std::make_shared<Texture>("res/pictures/color.png"));
	menuManager->addMenu("bottompanel", panel);
}

void registerPlayPanel() {
	std::shared_ptr<Menu> panel = std::make_shared<Menu>(window, renderer, systemInformation, screenResolution);
	panel->setEnabled(true);
	panel->setHeight(0.25f);
	panel->setWidth(1.0f);
	panel->setCoordinates(Point2D<float>(-1.0f, -0.6f));
	panel->setColor(glm::vec4(79.0f / 255.0f, 111.0f / 255.0f, 118.0f /255.0f, 0.0f));

	std::shared_ptr<Button> button = std::make_shared<Button>(renderer, -0.525, -1.1f, 1.0f, 1.1f);
	button->setButtonType(ButtonType::ROUND_RECT);
	button->setTextRenderer(textRendererRegistry->lookup("inter"));
	button->setBorderColorScheme(glm::vec4(1.0f, 1.0f, 1.0f, 0.25f));
	button->setColorScheme(glm::vec4(26.0f / 255.0f, 110.0f / 255.0f, 205.0f / 255.0f, 0.7f));
	button->setTextToRender("LAUNCH 1.8.9");
	button->setTextColorScheme(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	auto onClick = []() {
		glutHideWindow();
		hide = true;
	};
	button->setOnClick(onClick);
	button->setBorderEnabled(true);
	button->setCenterType(CenterType::BOTH);
	button->setBorderWidth(0.5f);
	button->setHoverColorScheme(glm::vec4(26.0f / 255.0f, 110.0f / 255.0f, 205.0f / 255.0f, 0.85f));

	panel->addButton(button);

	std::shared_ptr<TextRenderer> textRenderer = textRendererRegistry->lookup("montserrat");
	std::string playString = "PLAY NOW";
	std::shared_ptr<TextComponent> playTextComponent = std::make_shared<TextComponent>(button->getX() + button->getWidth() / 2 - textRenderer->getLengthOfString(playString, 1.0f), -0.4f, playString, renderer, textRenderer, screenResolution);
	playTextComponent->setTextColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

	textRenderer = textRendererRegistry->lookup("titillium_16");
	std::string subPlayString = "GO TO SETTINGS TO CHANGE VERSION OR EDIT DEDICATED RAM";
	std::shared_ptr<TextComponent> subPlayTextComponent = std::make_shared<TextComponent>(button->getX() + button->getWidth() / 2 - textRenderer->getLengthOfString(subPlayString, 1.0f), -0.7f, subPlayString, renderer, textRenderer, screenResolution);
	subPlayTextComponent->setTextColor(glm::vec4(185.0f / 255.0f, 185.0f / 255.0f, 185.0f / 255.0f, 1.0f));
	auto additionalDrawCall = [playTextComponent, subPlayTextComponent]() {
		playTextComponent->draw(playTextComponent->getCoordinates().getX(), playTextComponent->getCoordinates().getY());
		subPlayTextComponent->draw(subPlayTextComponent->getCoordinates().getX(), subPlayTextComponent->getCoordinates().getY());
	};
	panel->setAdditionalDrawCall(additionalDrawCall);
	std::shared_ptr<ButtonAnimation> animation = std::static_pointer_cast<ButtonAnimation>(std::make_shared<FadeInOutAnimation>(button, systemInformation, 0.025 * button->getWidth(), 0.025 * button->getHeight(), 125));
	std::shared_ptr<MultiAnimationInvoker> invoker = std::make_shared<MultiAnimationInvoker>();
	std::shared_ptr<AnimationInvoker> animationInvoker = std::make_shared<AnimationInvoker>();
	std::shared_ptr<AnimationTask> animationTask = std::make_shared<AnimationTask>(button, animation, -1);
	animationInvoker->push(animationTask);
	invoker->push(animationInvoker);
	panel->pushAnimation(button, invoker);

	std::shared_ptr<ButtonModifier> glow = std::static_pointer_cast<ButtonModifier>(std::make_shared<PersistentGlow>(screenResolution, glm::vec3(26.0f / 255.0f, 110.0f / 255.0f, 205.0f / 255.0f), 0.075f, 0.025f, 0.25f));
	panel->pushModifier(button, glow);

	menuManager->addMenu("playpanel", panel);
}

std::string formatBytes(unsigned long long bytes) {
	unsigned long long gb = 1024LL * 1024LL * 1024LL;
	unsigned long long tgb = bytes / gb;
	return std::to_string(tgb) + "GB";
}

void registerSettingsMenu() {
	std::shared_ptr<Panel> panel = std::make_shared<Panel>("", window, renderer, systemInformation, screenResolution);

	panel->setPanelHeight(0.2f);
	panel->setTextRenderer(textRendererRegistry->lookup("montserrat"));
	panel->setEnabled(false);
	panel->setHeight(1.7f);
	panel->setWidth(2.0f);
	panel->setCoordinates(Point2D<float>(-1.0f, -0.9f));
	panel->setTextColor(glm::vec4(1.0f, 1.0f, 1.0f, 0.25f));
	panel->setMask(0);
	panel->setPanelMask(0);
	panel->setColor(glm::vec4(18.0f / 255.0f, 19.0f / 255.0f, 21.0f / 255.0f, 0.75f));
	panel->setPanelColor(panel->getColor());
	panel->setAdditionalDrawCall([panel] {
		std::vector<std::shared_ptr<Button>> btns = *panel->getButtons().get();
		for (std::shared_ptr<Button> btn : btns) {
			if (btn->textToRender.find("Client") == std::string::npos) {
				continue;
			}
			if (!strcmp(btn->textToRender.c_str(), profile->getActiveClient().c_str())) {
				btn->setColorScheme(glm::vec4(135.0f / 255.0f, 196.0f / 255.0f, 137.0f / 255.0f, 0.2f));
			}
			else {
				btn->setColorScheme(glm::vec4(242.0f / 255.0f, 69.0f / 255.0f, 69.0f / 255.0f, 0.2f));
			}
		}
		renderer->drawRect(-0.3f, -0.125f, 0.6f, 0.5f, glm::vec4(1.0f, 1.0f, 1.0f, 0.25f));
		renderer->drawRect(-0.9f, -0.625f, 0.4f, 1.0f, glm::vec4(1.0f, 1.0f, 1.0f, 0.25f));
	});

	std::shared_ptr<SliderButton> button = std::make_shared<SliderButton>(screenResolution, renderer, 0.0f, 0.0f, 0.5f, 0.25f);
	button->setSliderRadius(0.075f);
	button->setSliderType(SliderType::CIRCLE);
	button->setSliderColorScheme(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	button->setColorScheme(glm::vec4(100.0f / 255.0f, 100.0f / 255.0f, 100.0f / 255.0f, 0.0f));
	button->setBarColorScheme(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	button->setSliderType(SliderType::CIRCLE);
	button->setSliderPosition((float)(profile->getDedicatedBytes() / (long double) systemInformation->getTotalRAMBytes()));
	button->setBorderEnabled(false);
	button->setBorderColorScheme(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	button->setHoverColorScheme(button->getColorScheme());
	button->setX(-0.255f);
	button->setY(0.0f);
	unsigned long long min = 2LL * 1024LL * 1024LL * 1024LL;
	float normalized = (long double) profile->getDedicatedBytes() / systemInformation->getTotalRAMBytes();
	button->setSliderPosition(normalized);
	button->setReleaseFunction([button, min]() {
		float normalized = MathUtils::max(0.0f, MathUtils::min(1.0f, (systemInformation->getMouseX() - button->getX()) / button->getWidth()));
		unsigned long long bytes = systemInformation->getTotalRAMBytes() * (long double)normalized;
		profile->setDedicatedBytes(bytes);
		button->setMouseDown(false);
		saveSettings();
	});

	std::shared_ptr<Button> exodusButton = std::make_shared<Button>(renderer, -0.85f, 0.0f, 0.3f, 0.2f);
	exodusButton->setTextToRender("Exodus Client");
	exodusButton->setTextRenderer(textRendererRegistry->lookup("titillium"));
	exodusButton->setColorScheme(glm::vec4(135.0f / 255.0f, 196.0f / 255.0f, 137.0f / 255.0f, 0.2f));
	exodusButton->setTextColorScheme(glm::vec4(205.0f / 255.0f, 205.0f / 255.0f, 205.0f / 255.0f, 0.85f));
	exodusButton->setCenterType(CenterType::BOTH);
	exodusButton->setButtonType(ButtonType::RECT);
	exodusButton->setHoverColorScheme(glm::vec4(73.0f / 255.0f, 212.0f / 255.0f, 189.0f / 255.0f, 0.2f));
	exodusButton->setOnClick([] { profile->setActiveClient("Exodus Client"); });

	std::shared_ptr<Button> demonicButton = std::make_shared<Button>(renderer, -0.85f, -0.3f, 0.3f, 0.2f);
	demonicButton->setTextToRender("Demonic Client");
	demonicButton->setTextRenderer(textRendererRegistry->lookup("titillium"));
	demonicButton->setColorScheme(glm::vec4(242.0f / 255.0f, 69.0f / 255.0f, 69.0f / 255.0f, 0.2f));
	demonicButton->setTextColorScheme(glm::vec4(205.0f / 255.0f, 205.0f / 255.0f, 205.0f / 255.0f, 0.85f));
	demonicButton->setCenterType(CenterType::BOTH);
	demonicButton->setButtonType(ButtonType::RECT);
	demonicButton->setHoverColorScheme(glm::vec4(73.0f / 255.0f, 212.0f / 255.0f, 189.0f / 255.0f, 0.2f));
	demonicButton->setOnClick([] { profile->setActiveClient("Demonic Client"); });

	std::shared_ptr<Button> cosmicButton = std::make_shared<Button>(renderer, -0.85f, -0.6f, 0.3f, 0.2f);
	cosmicButton->setTextToRender("Cosmic Client");
	cosmicButton->setTextRenderer(textRendererRegistry->lookup("titillium"));
	cosmicButton->setColorScheme(glm::vec4(242.0f / 255.0f, 69.0f / 255.0f, 69.0f / 255.0f, 0.2f));
	cosmicButton->setTextColorScheme(glm::vec4(205.0f / 255.0f, 205.0f / 255.0f, 205.0f / 255.0f, 0.85f));
	cosmicButton->setCenterType(CenterType::BOTH);
	cosmicButton->setButtonType(ButtonType::RECT);
	cosmicButton->setHoverColorScheme(glm::vec4(73.0f / 255.0f, 212.0f / 255.0f, 189.0f / 255.0f, 0.2f));
	cosmicButton->setOnClick([] { profile->setActiveClient("Cosmic Client"); });


	std::shared_ptr<TextRenderer> textRenderer = textRendererRegistry->lookup("titillium_24");
	std::string clientString = "Choose Client";
	std::shared_ptr<TextComponent> clientTextComponent = std::make_shared<TextComponent>(-0.85f + exodusButton->getWidth() / 2 - textRenderer->getLengthOfString(clientString, 1.0f), 0.4f, clientString, renderer, textRenderer, screenResolution);
	clientTextComponent->setTextColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

	textRenderer = textRendererRegistry->lookup("titillium_16");
	clientString = "What client will be Launched";
	std::shared_ptr<TextComponent> subClientTextComponent = std::make_shared<TextComponent>(-0.85f + exodusButton->getWidth() / 2 - textRenderer->getLengthOfString(clientString, 1.0f), 0.3f, clientString, renderer, textRenderer, screenResolution);
	subClientTextComponent->setTextColor(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));

	panel->pushComponent(std::make_shared<ButtonComponent>(exodusButton));
	panel->pushComponent(std::make_shared<ButtonComponent>(demonicButton));
	panel->pushComponent(std::make_shared<ButtonComponent>(cosmicButton));
	panel->pushComponent(std::make_shared<ButtonComponent>(button));

	textRenderer = textRendererRegistry->lookup("titillium_24");
	std::string memoryString = "Allocated Memory";
	std::shared_ptr<TextComponent> memoryTextComponent = std::make_shared<TextComponent>(-0.255f + button->getWidth() / 2 - textRenderer->getLengthOfString(memoryString, 1.0f), 0.4f, memoryString, renderer, textRenderer, screenResolution);
	memoryTextComponent->setTextColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

	textRenderer = textRendererRegistry->lookup("titillium_16");
	memoryString = "How much memory should be allocated to the game";
	std::shared_ptr<TextComponent> subMemoryTextComponent = std::make_shared<TextComponent>(-0.255f + button->getWidth() / 2 - textRenderer->getLengthOfString(memoryString, 1.0f), 0.3f, memoryString, renderer, textRenderer, screenResolution);
	subMemoryTextComponent->setTextColor(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));

	textRenderer = textRendererRegistry->lookup("titillium_16");
	std::shared_ptr<TextComponent> allocatedTextComponent = std::make_shared<TextComponent>(0.0f, 0.0f, memoryString, renderer, textRenderer, screenResolution);
	allocatedTextComponent->setTextColor(glm::vec4(0.84f, 0.85f, 0.85f, 1.0f));
	auto func = [allocatedTextComponent, button, textRenderer]() {
		std::string text = formatBytes(profile->getDedicatedBytes()) + "/" + formatBytes(systemInformation->getTotalRAMBytes()) + " Allocated";
		Point2D<float> coordinates = Point2D<float>(-0.255f + button->getWidth() / 2 - textRenderer->getLengthOfString(text, 1.0f), -0.05f);
		allocatedTextComponent->setCoordinates(coordinates);
		return text;
	};
	allocatedTextComponent->setStringFunction(func);

	panel->pushComponent(memoryTextComponent);
	panel->pushComponent(subMemoryTextComponent);
	panel->pushComponent(allocatedTextComponent);
	panel->pushComponent(clientTextComponent);
	panel->pushComponent(subClientTextComponent);

	menuManager->addMenu("settingsmenu", panel);
}

void registerMainMenu() {
	std::shared_ptr<Menu> menu = std::make_shared<Menu>(window, renderer, systemInformation, screenResolution);
	menu->setEnabled(false);
	std::string path = "res/pictures/background.png";
	menu->bindTexture(std::make_shared<Texture>(path));
	menu->setHeight(1.7f);
	menu->setWidth(1.0f);
	menu->setCoordinates(Point2D<float>(-1.0f, -0.9f));
	menu->setColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));

	auto drawCall = []() {
		//renderer->drawRect(-1.0f, 0.7, 1.0f, 0.1f, glm::vec4(40.0f / 255.0f, 71.0f / 255.0f, 195.0f / 255.0f, 0.8f));
		//renderer->drawRect(-0.015f, -0.9f, 0.015f, 1.6f, glm::vec4(40.0f / 255.0f, 71.0f / 255.0f, 195.0f / 255.0f, 0.8f));
	};
	menu->setAdditionalDrawCall(drawCall);

	std::shared_ptr<Button> exoFatButton = std::make_shared<Button>(renderer, -0.625f, -0.4f, 1.2f, 1.2f);
	exoFatButton->setButtonType(ButtonType::RECT);
	exoFatButton->setBorderColorScheme(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
	exoFatButton->setColorScheme(glm::vec4(26.0f / 255.0f, 110.0f / 255.0f, 205.0f / 255.0f, 0.0f));
	exoFatButton->setHoverColorScheme(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
	exoFatButton->bindTexture(std::make_shared<Texture>("res/pictures/exofat.png"));

	menu->addButton(exoFatButton);

	menuManager->addMenu("mainmenu", menu);
}

void registerOtherMainMenu() {
	std::shared_ptr<Menu> menu = std::make_shared<Menu>(window, renderer, systemInformation, screenResolution);
	menu->setEnabled(false);
	std::string path = "res/pictures/other_main_background.png";
	menu->bindTexture(std::make_shared<Texture>(path));
	menu->setHeight(1.7f);
	menu->setWidth(1.0f);
	menu->setCoordinates(Point2D<float>(0.0f, -0.9f));
	menu->setColor(glm::vec4(18.0f / 255.0f, 19.0f / 255.0f, 21.0f / 255.0f, 0.0f));


	std::shared_ptr<Button> astroButton = std::make_shared<Button>(renderer, 0.465f, -0.975f, 0.4f, 0.4f);
	astroButton->setButtonType(ButtonType::RECT);
	astroButton->setBorderColorScheme(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
	astroButton->setColorScheme(glm::vec4(26.0f / 255.0f, 110.0f / 255.0f, 205.0f / 255.0f, 0.0f));
	astroButton->setHoverColorScheme(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
	astroButton->bindTexture(std::make_shared<Texture>("res/pictures/astropvp.png"));
	astroButton->setOnClick([] {
		profile->setActiveClient("Cosmic Client");
		playingServer = "play.astropvp.net";
		glutHideWindow();
		hide = true;
	});

	std::shared_ptr<Button> demonicButton = std::make_shared<Button>(renderer, 0.53f, -0.585f, 0.31f, 0.25f);
	demonicButton->setButtonType(ButtonType::RECT);
	demonicButton->setBorderColorScheme(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
	demonicButton->setColorScheme(glm::vec4(26.0f / 255.0f, 110.0f / 255.0f, 205.0f / 255.0f, 0.0f));
	demonicButton->setHoverColorScheme(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
	demonicButton->bindTexture(std::make_shared<Texture>("res/pictures/demonicpvp.png"));
	demonicButton->setOnClick([] {
		profile->setActiveClient("Demonic Client"); 
		playingServer = "play.demonicpvp.net";
		glutHideWindow();
		hide = true;
	});

	std::shared_ptr<Button> cosmicButton = std::make_shared<Button>(renderer, 0.435f, -0.3f, 0.5f, 0.35f);
	cosmicButton->setButtonType(ButtonType::RECT);
	cosmicButton->setBorderColorScheme(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
	cosmicButton->setColorScheme(glm::vec4(0.0f / 255.0f, 37.0f / 255.0f, 78.0f / 255.0f, 0.0f));
	cosmicButton->setHoverColorScheme(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
	cosmicButton->bindTexture(std::make_shared<Texture>("res/pictures/cosmicpvp.png"));
	cosmicButton->setOnClick([] {
		profile->setActiveClient("Cosmic Client");
		playingServer = "cosmicpvp.me";
		glutHideWindow();
		hide = true;
	});

	auto addDrawCall = []() {
		renderer->drawRect(-1.0f, 0.7, 2.0f, 0.1f, glm::vec4(40.0f / 255.0f, 71.0f / 255.0f, 195.0f / 255.0f, 1.0f));
		renderer->drawRect(-0.01f, -0.9f, 0.02f, 1.6f, glm::vec4(40.0f / 255.0f, 71.0f / 255.0f, 195.0f / 255.0f, 1.0f));
		renderer->renderImage(0.3f, 0.8f, 0.685f, -0.875f, *std::make_shared<Texture>("res/pictures/gray_rect.png").get());
		//renderer->renderImage(0.4f, 0.07f, -0.2f, 0.715f, *std::make_shared<Texture>("res/pictures/exodusclient_text.png").get());
		//TextRenderer* textRenderer = textRendererRegistry->lookup("walkthemoon_24").get();
		//textRenderer->renderText(*shaderRegistry->lookup("text").get(), "Quick Connect", 0.685f, -0.05f, 24, glm::vec3(0.4f, 0.4f, 0.4f));
		//renderer->renderImage(0.07f, 0.09f, -0.8f, 0.699f, *std::make_shared<Texture>("res/pictures/lightninge.png").get());
	};

	/*
	std::shared_ptr<Button> panelHeaderButton = std::make_shared<Button>(renderer, -0.9f, -0.16f, 1.2f, 0.125f);
	panelHeaderButton->setButtonType(ButtonType::TOP_ROUND_RECT);
	panelHeaderButton->setBorderColorScheme(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
	panelHeaderButton->setColorScheme(glm::vec4(47.0f / 255.0f, 130.0f / 255.0f, 225.0f / 255.0f, 1.0f));
	panelHeaderButton->setHoverColorScheme(panelHeaderButton->getColorScheme());
	panelHeaderButton->setTextToRender("Updates & Bug Fixes");
	panelHeaderButton->setCenterType(CenterType::BOTH);
	panelHeaderButton->setTextRenderer(textRendererRegistry->lookup("walkthemoon"));

	std::shared_ptr<Button> panelBody = std::make_shared<Button>(renderer, -0.9f, panelHeaderButton->getY() - panelHeaderButton->getHeight() - 0.675f, 1.2f, 0.8f);
	panelBody->setButtonType(ButtonType::RECT);
	panelBody->setBorderColorScheme(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
	panelBody->setColorScheme(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	panelBody->setHoverColorScheme(panelBody->getColorScheme());*/

//	menu->addButton(panelHeaderButton);
	menu->addButton(astroButton);
	menu->addButton(demonicButton);
	menu->addButton(cosmicButton);
	//menu->addButton(panelBody);
	menu->setAdditionalDrawCall(addDrawCall);

	menuManager->addMenu("other_mainmenu", menu);
}

void registerMenus() {
	menuManager = std::make_shared<MenuManager>();
	registerMainMenu();
	registerOtherMainMenu();
	registerPlayPanel();
	registerBottomPanel();
	registerHeaderPanel();
	registerFooterPanel();
	registerSettingsMenu();
}

void initializeSwappables() {
	swappableComponent = std::make_shared<SwappableComponent<std::string>>();
}

void testAuthentication() {
	AuthRequest request = AuthRequest(0);
	request.setPassword("");
	request.setUserCredentials("asdfasd@gmail.com");
	request.post();
}

void downloadContent() {
	DownloadContent content;
	content.downloadContent();
}

void initialize() {
	accountManager = std::make_shared<Minecraft::AccountManager>();
	profile = LauncherSettings::loadFromFile("profilesettings.json");
	if (profile->getDedicatedBytes() < 2LL * 1024LL * 1024LL * 1024LL) {
		profile->setDedicatedBytes(2LL * 1024LL * 1024LL * 1024LL);
	}
	shaderRegistry = std::make_shared<ShaderRegistry>();
	shaderRegistry->bind("basic", std::make_shared<Shader>("res/shaders/Basic.shader"));
	shaderRegistry->bind("text", std::make_shared<Shader>("res/shaders/Font.shader"));
	shaderRegistry->bind("texture", std::make_shared<Shader>("res/shaders/Texture.shader"));
	textRendererRegistry = std::make_shared<TextRendererRegistry>();
	textRendererRegistry->bind("font", std::make_shared<TextRenderer>(screenResolution, "res/fonts/robotobold.ttf", 22));
	textRendererRegistry->bind("walkthemoon", std::make_shared<TextRenderer>(screenResolution, "res/fonts/walkthemoon.ttf", 14));
	textRendererRegistry->bind("walkthemoon_24", std::make_shared<TextRenderer>(screenResolution, "res/fonts/walkthemoon.ttf", 24));
	textRendererRegistry->bind("oswald", std::make_shared<TextRenderer>(screenResolution, "res/fonts/oswald.ttf", 30));
	textRendererRegistry->bind("quicksand", std::make_shared<TextRenderer>(screenResolution, "res/fonts/quicksand.ttf", 18));
	textRendererRegistry->bind("titillium", std::make_shared<TextRenderer>(screenResolution, "res/fonts/titillium.ttf", 20));
	textRendererRegistry->bind("montserrat", std::make_shared<TextRenderer>(screenResolution, "res/fonts/montserrat.ttf", 25));
	textRendererRegistry->bind("titillium_40", std::make_shared<TextRenderer>(screenResolution, "res/fonts/titillium.ttf", 40));
	textRendererRegistry->bind("titillium_24", std::make_shared<TextRenderer>(screenResolution, "res/fonts/titillium.ttf", 24));
	textRendererRegistry->bind("titillium_16", std::make_shared<TextRenderer>(screenResolution, "res/fonts/titillium.ttf", 16));
	textRendererRegistry->bind("indieflower", std::make_shared<TextRenderer>(screenResolution, "res/fonts/indieflower.ttf", 30));
	textRendererRegistry->bind("notoserif", std::make_shared<TextRenderer>(screenResolution, "res/fonts/notoserif.ttf", 26));
	textRendererRegistry->bind("inter", std::make_shared<TextRenderer>(screenResolution, "res/fonts/inter.ttf", 18));
	textRendererRegistry->bind("crimsontext", std::make_shared<TextRenderer>(screenResolution, "res/fonts/crimsontext.ttf", 24));
	textRendererRegistry->bind("anton", std::make_shared<TextRenderer>(screenResolution, "res/fonts/anton.ttf", 18));
	textRendererRegistry->bind("roboto", std::make_shared<TextRenderer>(screenResolution, "res/fonts/robotobold.ttf", 26));
	renderer = std::make_shared<Renderer>(textRendererRegistry->lookup("font"), shaderRegistry->lookup("basic"), shaderRegistry->lookup("text"), shaderRegistry->lookup("texture"));
	systemInformation = std::make_shared<SystemInformation>();
	::ShowWindow(::GetConsoleWindow(), SW_SHOW);
	glutPassiveMotionFunc(onMouseMove);
	glutMotionFunc(onMouseMove);
	glutMouseFunc(onMouseClick);
	glutMouseWheelFunc(onMouseWheel);
	initializeSwappables();
	registerMenus();
} 

int main(int argc, char** argv) {
	downloadContent();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	screenResolution = std::make_shared<ScreenResolution>(1300, 800);
	glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - screenResolution->getResolutionWidth()) / 2,
		(glutGet(GLUT_SCREEN_HEIGHT) - screenResolution->getResolutionHeight()) / 2);
	glutInitDisplayMode(GL_DOUBLE|GLUT_BORDERLESS);
	glutInitWindowSize(screenResolution->getResolutionWidth(), screenResolution->getResolutionHeight());
	window = glutCreateWindow("Exodus Launcher");
	if (glewInit() != GLEW_OK) {
		std::cout << "Could not init GLEW" << std::endl;
		return -1;
	}
	FreeConsole();
	initialize();
	glutDisplayFunc(render);
	glutTimerFunc(100, update, 0);
	glutMainLoop();
	return 0;
}
