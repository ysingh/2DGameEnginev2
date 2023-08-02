#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include <algorithm>
#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../AssetStore/AssetStore.h"
#include "SDL.h"

class RenderSystem : public System {
public:
	RenderSystem() {
		RequireComponent<TransformComponent>();
		RequireComponent<SpriteComponent>();
	}

	void Render(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore) {
		auto entities = GetSystemEntities();
		// Sort the entities by order of z index here, in every frame
		// this might not be good enough
		std::sort(entities.begin(), entities.end(), [](Entity a, Entity b) {
			return a.GetComponent<SpriteComponent>().zIndex < b.GetComponent<SpriteComponent>().zIndex;
		});
		for (auto entity : entities) {
			const auto transform = entity.GetComponent<TransformComponent>();
			const auto sprite = entity.GetComponent<SpriteComponent>();
			
			//SDL_SetRenderDrawColor(renderer, sprite.color.r, sprite.color.g, sprite.color.b, sprite.color.a);
			

			// set the src rect
			SDL_Rect srcRect = {
				0,
				0,
				
			};

			// set the destination rect with the x, y position to be rendered
			SDL_Rect destRect = {
				static_cast<int>(transform.position.x),
				static_cast<int>(transform.position.y),
				static_cast<int>(sprite.width * transform.scale.x),
				static_cast<int>(sprite.height * transform.scale.y),
			};

			SDL_RenderCopyEx(renderer,
				assetStore->GetTexture(sprite.assetId), 
				&(sprite.srcRect),
				&destRect,
				transform.rotation,
				NULL,
				SDL_FLIP_NONE);

			//SDL_RenderFillRect(renderer, &destRect);
		}
	}
};

#endif