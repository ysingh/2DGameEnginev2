#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include "glm/glm.hpp"

struct SpriteComponent {
	SDL_Surface* surface;
	int width;
	int height;
	glm::vec4 color;

	SpriteComponent(std::string path = "", int width = 0, int height = 0, glm::vec4 color = glm::vec4(255, 255, 255, 255)) {
		this->width = width;
		this->height = height;
		this->color = color;

		//SDL_Surface* surface =  IMG_Load(path.c_str());
	}
};
#endif