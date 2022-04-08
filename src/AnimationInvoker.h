#pragma once
#include "Queue.h"
#include "AnimationTask.h"

class AnimationInvoker {
private:
	std::shared_ptr<Queue<std::shared_ptr<AnimationTask>>> queue;

public:
	AnimationInvoker();
	Queue<std::shared_ptr<AnimationTask>>* getQueue();
	void push(std::shared_ptr<AnimationTask> animationTask);
	void process();
	bool canProcess();
};