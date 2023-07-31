#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "SDL.h"

class RenderSystem : public System {
public:
	RenderSystem() {
		RequireComponent<TransformComponent>();
		RequireComponent<SpriteComponent>();
	}

	void Render(SDL_Renderer* renderer) {
		for (auto entity : GetSystemEntities()) {
			const auto transform = entity.GetComponent<TransformComponent>();
			const auto sprite = entity.GetComponent<SpriteComponent>();
			
			//SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, sprite.surface);
			SDL_SetRenderDrawColor(renderer, sprite.color.r, sprite.color.g, sprite.color.b, sprite.color.a);
			SDL_Rect destRect = {
				static_cast<int>(transform.position.x),
				static_cast<int>(transform.position.y),
				sprite.width,
				sprite.height
			};

			SDL_RenderFillRect(renderer, &destRect);
		}
	}
};

#endif