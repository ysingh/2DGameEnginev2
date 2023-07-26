#include "Game.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

glm::vec2 playerPos;
glm::vec2 playerVelocity;

Game::Game() {
	isRunning = false;
	Logger::Log("Game constructor called");
}

Game::~Game() {
	Logger::Log("Game destructor called");
}

void Game::Initialize() {
	if (SDL_Init(SDL_INIT_EVERYTHING)) {
		Logger::Err("Error initializing SDL");
		Logger::Err(SDL_GetError());
		return;
	}

	SDL_DisplayMode displayMode;
	if (SDL_GetCurrentDisplayMode(0, &displayMode) != 0) {
		Logger::Err("Error getting SDL display mode");
		Logger::Err(SDL_GetError());
	}
	// Better to not scale the window to the users display.
	// Instead set window mode to fullscreen and let SDL scale the fixed window to that size
	// The difference is if we scale the window users with higher width and height displays will see more of the game
	// because we will be trying to fill all those pixels with game data
	// 
	// If we keep window size fixed and let sdl scale it then all resolutions will see the same size
	windowWidth = WINDOW_WIDTH; //displayMode.w;
	windowHeight = WINDOW_HEIGHT; //displayMode.h;

	// passing NULL for the first param creates a window without titlebar etc.
	// https://wiki.libsdl.org/SDL2/SDL_CreateWindow
	//uint32_t flags = SDL_WINDOW_FULLSCREEN;
		// SDL_WINDOW_BORDERLESS |  SDL_WINDOW_MAXIMIZED | SDL_WINDOW_INPUT_GRABBED | SDL_WINDOW_ALLOW_HIGHDPI;
	window = SDL_CreateWindow(NULL,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		windowWidth,
		windowHeight,
		0);

	if (!window) {
		Logger::Err("Error creating SDL window");
		Logger::Err(SDL_GetError());
		return;
	}

	// Let SDL set the displayMode to FULLSCREEN
	// and scale our window to be the size of the screen (as big as possible while preserving aspect ratio)
	// Is this the same as setting SDL_CreateWindow flag to SDL_WINDOW_FULLSCREEN

	//SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

	// https://wiki.libsdl.org/SDL2/SDL_CreateRenderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer) {
		Logger::Err("Error Creating SDL Renderer");
		Logger::Err(SDL_GetError());
		return;
	}
	// Set the renderer's draw color
	SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);

	isRunning = true;
}

void Game::Destroy() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Game::Setup() {
	playerPos = glm::vec2(10.0, 20.0);
	playerVelocity = glm::vec2(200, 100);
}

void Game::Run() {
	Setup();
	while (isRunning) {
		ProcessInput();
		Update();
		Render();
	}
}

void Game::ProcessInput() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			isRunning = false;
			break;
		case SDL_MOUSEMOTION:
			//Logger::Log("We got a mouse event.
			//Logger::Log(" Current mouse position " + event.motion.x + " " + event.motion.y);
			break;
		case SDL_KEYDOWN:
			Logger::Log("KEY PRESS.");
			// TODO(yudi): This does not work as event.key.keysym.sym is not a string
			//Logger::Log(" Current key pressed: " + event.key.keysym.sym);
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				isRunning = false;
			}
			break;
		case SDL_KEYUP:
			Logger::Log("KEY RELEASED.");
			// TODO(yudi): This does not work as event.key.keysym.sym is not a string
			//Logger::Log(" Current key released: " + event.key.keysym.sym);
			break;
			/*default:
				std::cout << "Unhandled event" << std::endl;
				std::cout << event.type << std::endl;
			break;*/
		}
	}
}

void Game::Update() {
	// If we are too fast waste some time - this caps the framerate
	// Commenting this out results in us running at uncapped FPS
	int timeToWait = MILLISECONDS_PER_FRAME - (SDL_GetTicks() - millisecsPreviousFrame);
	if (timeToWait > 0 && timeToWait <= MILLISECONDS_PER_FRAME) {
		/*
		The function SDL_Delay is not incredibly accurate, since the call of SDL_Delay itself takes some time to execute.
		For example, SDL_Delay will never work at a finer resolution than what the OS's scheduler offers.
		In the 90's a normal Linux scheduler had a 10 millisecond resolution, which meant SDL_Delay would sleep for at least ten times longer than requested.
		Modern Linux distros have a 1ms resolution, but the underlying idea still holds.
		And load is also a factor, meaning that if a system is heavily loaded, we might sleep for hundreds of milliseconds.
		*/
		SDL_Delay(timeToWait);
	}

	// Number of seconds elapsed since the last frame
	double deltaTime = (SDL_GetTicks() - millisecsPreviousFrame) / 1000.f;

	// Store the current frame time
	millisecsPreviousFrame = SDL_GetTicks();

	playerPos.x += playerVelocity.x * deltaTime;
	playerPos.y += playerVelocity.y * deltaTime;
}


void Game::Render() {
	// This is now being set once in the init function since it shouldn't change frame by frame
	// SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);

	// It's recommended to clear the rederer before redrawing the current frame
	SDL_RenderClear(renderer);

	SDL_Surface* surface = IMG_Load("./assets/images/tank-tiger-right.png");
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	// Can free the surface if not needed again
	SDL_FreeSurface(surface);

	SDL_Rect destRect = {
		static_cast<int>(playerPos.x),
		static_cast<int>(playerPos.y),
		32,
		32 };
	//SDL_Rect srcRect = { 288 * frame, 128, 288, 128 };
	SDL_RenderCopy(renderer, texture, NULL, &destRect);


	SDL_DestroyTexture(texture);

	SDL_RenderPresent(renderer);
}
