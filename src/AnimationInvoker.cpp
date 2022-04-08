#include "AnimationInvoker.h"

AnimationInvoker::AnimationInvoker() {
	queue = std::make_shared<Queue<std::shared_ptr<AnimationTask>>>();
}

Queue<std::shared_ptr<AnimationTask>>* AnimationInvoker::getQueue() {
	return queue.get();
}

bool AnimationInvoker::canProcess() {
	return !getQueue()->isEmpty();
}

void AnimationInvoker::push(std::shared_ptr<AnimationTask> animationTask) {
	queue->push(animationTask);
}

void AnimationInvoker::process() {
	if (!canProcess()) {
		return;
	}
	AnimationTask* task = getQueue()->peek().get();
	ButtonAnimation* animation = task->getAnimation();
	if (task->canTerminate()) {
		animation->terminate();
		getQueue()->pop();
		return;
	}
	task->process();
}