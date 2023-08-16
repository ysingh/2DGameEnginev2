#ifndef ANIMATIONCOMPONENT_H
#define ANIMATIONCOMPONENT_H

#include <SDL.h>
#include <string>

struct AnimationComponent {
	std::string animationName;
	int numFrames;
	int startFrameXOffset;
	int startFrameYOffset;
	int currentFrame;
	int frameRate;
	bool isLoop;
	int startTime;
	bool play;

	AnimationComponent(std::string animationName = "", int numFrames = 1, int frameRate = 1, bool isLoop = true, int startFrameXOffset = 0, int startFrameYOffset = 0) {
		this->animationName = animationName;
		this->numFrames = numFrames;
		this->currentFrame = 1;
		this->frameRate = frameRate;
		this->isLoop = isLoop;
		this->startTime = SDL_GetTicks();
		this->play = true;
		this->startFrameXOffset = startFrameXOffset;
		this->startFrameYOffset = startFrameYOffset;
	}

};

#endif
