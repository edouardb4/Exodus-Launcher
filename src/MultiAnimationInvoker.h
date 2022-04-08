#pragma once
#include "AnimationInvoker.h"

class MultiAnimationInvoker {
public:
	MultiAnimationInvoker();
	void push(std::shared_ptr<AnimationInvoker> invoker);
	bool canProcess();
	void process();
private:
	std::shared_ptr<std::list<std::shared_ptr<AnimationInvoker>>> invokers;
};