#include "AnimationTask.h"
#include "MathUtils.h"

#include <time.h>
#include <ctime>

AnimationTask::AnimationTask(std::shared_ptr<Button> btn, std::shared_ptr<ButtonAnimation> animation, int lifespan) {
	this->animation = animation;
	this->btn = btn;
	this->lifespan = lifespan;
	this->start = time(nullptr) * 1000;
	this->end = this->start + lifespan;
}

bool AnimationTask::canTerminate() {
	if (lifespan <= 0) {
		return false;
	}
	float normalized = MathUtils::normalize(getMilisecondsElapsed(), 0, getLifeSpan());
	return getAnimation()->canTerminate(normalized);
}

ButtonAnimation* AnimationTask::getAnimation() {
	return animation.get();
}

Button* AnimationTask::getButton() {
	return btn.get();
}

int AnimationTask::getMilisecondsElapsed() {
	return (int) (time(nullptr) * 1000 - this->start);
}

int AnimationTask::getLifeSpan() {
	return lifespan;
}

void AnimationTask::process() {
	float normalized = animation->getNormalized();
	ButtonAnimation* animation = getAnimation();
	if (animation->canTerminate(normalized)) {
		animation->terminate();
		return;
	}
	animation->run(normalized);
}