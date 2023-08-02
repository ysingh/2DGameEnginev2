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
			if (animation.play) {
				auto& sprite = entity.GetComponent<SpriteComponent>();
				float timeinSeconds = (SDL_GetTicks() - animation.startTime) / 1000.0;
				animation.currentFrame = (int)(timeinSeconds * animation.frameRate);
				if (animation.isLoop) {
					animation.currentFrame %= animation.numFrames;
					sprite.srcRect.x = sprite.width * animation.currentFrame;
				}
				else if (animation.currentFrame > animation.numFrames) {
					animation.currentFrame = 1;
					sprite.srcRect.x = 0;
					animation.play = false;
				}
				else {
					sprite.srcRect.x = sprite.width * animation.currentFrame;
				}
			}
		}
	}


	
};


#endif