#ifndef ANIMATIONSYSTEM_H
#define ANIMATIONSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/AnimationComponent.h"
#include "../Components/SpriteComponent.h"

class AnimationSystem : public System {
public:
	AnimationSystem() {
		RequireComponent<AnimationComponent>();
		RequireComponent<SpriteComponent>();
	}

	void Update(double deltaTime) {
		for (auto entity : GetSystemEntities()) {
			auto& animation = entity.GetComponent<AnimationComponent>();
			auto& sprite = entity.GetComponent<SpriteComponent>();
			int currentTicks = SDL_GetTicks();
			float animationPlayedForSeconds = (currentTicks - animation.startTime) / 1000.0;
			if (animationPlayedForSeconds - 1.0/animation.frameRate > 0) {
				animation.currentFrame = (animation.currentFrame + 1) % animation.numFrames;
				sprite.srcRect.x = sprite.width * animation.currentFrame;
				animation.startTime = SDL_GetTicks();
			}
		}
	}


	
};


#endif