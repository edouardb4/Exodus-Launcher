#pragma once

#include "SystemInformation.h"
#include <memory>

class ButtonAnimation {
protected:
	std::shared_ptr<SystemInformation> systemInformation;
public:
	ButtonAnimation(std::shared_ptr<SystemInformation> systemInformation);
	virtual bool canTerminate(float normalized);
	virtual void terminate();
	virtual float getNormalized();
	virtual void run(float normalized);
};