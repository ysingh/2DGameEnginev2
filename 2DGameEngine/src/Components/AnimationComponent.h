#ifndef ANIMATIONCOMPONENT_H
#define ANIMATIONCOMPONENT_H

#include <SDL.h>

struct AnimationComponent {
	int numFrames;
	int currentFrame;
	int frameRate;
	bool isLoop;
	int startTime;

	AnimationComponent(int numFrames = 1, int frameRate = 1, bool isLoop = true) {
		this->numFrames = numFrames;
		this->currentFrame = 1;
		this->frameRate = frameRate;
		this->isLoop = isLoop;
		this->startTime = SDL_GetTicks();
	}

};

#endif
