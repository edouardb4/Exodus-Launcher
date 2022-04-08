#include "Button.h"
#include "ButtonAnimation.h"

class AnimationTask {
protected:
	std::shared_ptr<Button> btn;
	std::shared_ptr<ButtonAnimation> animation;
	long start;
	long end;
	int lifespan;
	std::function<bool()> terminateFunction;

public:
	AnimationTask(std::shared_ptr<Button> btn, std::shared_ptr<ButtonAnimation> animation, int lifespan);
	int getLifeSpan();
	int getMilisecondsElapsed();
	bool canTerminate();
	Button* getButton();
	ButtonAnimation* getAnimation();
	void process();
	template<typename Func>
	void setTerminateFunction(Func func) {
		terminateFunction = func;
	}
};