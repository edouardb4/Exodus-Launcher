#include "SystemInformation.h"
#include <freeglut.h>

float SystemInformation::getMouseX() {
	return mouseX;
}

float SystemInformation::getMouseY() {
	return mouseY;
}

void SystemInformation::setMouseX(float mouseX) {
	this->mouseX = mouseX;
}

void SystemInformation::setMouseY(float mouseY) {
	this->mouseY = mouseY;
}

unsigned long long SystemInformation::getTotalRAMBytes() {
    typedef BOOL(WINAPI* PGMSE)(LPMEMORYSTATUSEX);
    PGMSE pGMSE = (PGMSE)GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), "GlobalMemoryStatusEx");
    if (pGMSE != 0)
    {
        MEMORYSTATUSEX mi;
        memset(&mi, 0, sizeof(MEMORYSTATUSEX));
        mi.dwLength = sizeof(MEMORYSTATUSEX);
        if (pGMSE(&mi))
            return mi.ullTotalPhys;
        else
            pGMSE = 0;
    }
    if (pGMSE == 0)
    {
        MEMORYSTATUS mi;
        memset(&mi, 0, sizeof(MEMORYSTATUS));
        mi.dwLength = sizeof(MEMORYSTATUS);
        GlobalMemoryStatus(&mi);
        return mi.dwTotalPhys;
    }
}