#include "MultiAnimationInvoker.h"

MultiAnimationInvoker::MultiAnimationInvoker() {
	this->invokers = std::make_shared<std::list<std::shared_ptr<AnimationInvoker>>>();
}

void MultiAnimationInvoker::process() {
	std::list<std::shared_ptr<AnimationInvoker>>* invokers = this->invokers.get();
	invokers->remove_if([](std::shared_ptr<AnimationInvoker> invoker) {
		return !invoker.get()->canProcess();
	});
	for (std::list<std::shared_ptr<AnimationInvoker>>::iterator it = invokers->begin(); it != invokers->end(); it++) {
		AnimationInvoker* invoker = it->get();
		invoker->process();
	}
}

bool MultiAnimationInvoker::canProcess() {
	return invokers.get()->size() != 0;
}

void MultiAnimationInvoker::push(std::shared_ptr<AnimationInvoker> invoker) {
	invokers->push_back(invoker);
}