#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include "glm/glm.hpp"
#include <SDL.h>
#include <string>

struct SpriteComponent {
	std::string assetId;
	int width;
	int height;
	SDL_Rect srcRect; 
	glm::vec4 color;

	SpriteComponent(std::string assetId = "",
		int width = 0,
		int height = 0,
		int srcRectX = 0,
		int srcRectY = 0,
		glm::vec4 color = glm::vec4(255, 255, 255, 255)
	){
		this->assetId = assetId;
		this->width = width;
		this->height = height;
		this->color = color;
		srcRect = { srcRectX, srcRectY, width, height };
	}
};
#endif