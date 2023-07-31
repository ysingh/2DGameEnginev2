#ifndef GAME_H
#define GAME_H
#include <SDL.h>
#include <memory>
#include "../ECS/ECS.h"

const int FPS = 60;
const int MILLISECONDS_PER_FRAME = 1000 / FPS;

class Game {
private:
	bool isRunning;
	SDL_Window* window;
	SDL_Renderer* renderer;
	int millisecsPreviousFrame = 0;
	std::unique_ptr<Registry> registry;

public:
	Game();
	~Game();
	void Initialize();
	void Destroy();
	void Run();
	void Setup();
	void ProcessInput();
	void Update();
	void Render();
	int windowWidth;
	int windowHeight;
	int refreshRate = 60;
};

#endif
