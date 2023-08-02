#include <glm/glm.hpp>
#include <SDL_image.h>
#include <fstream>
#include "../Logger/Logger.h"
#include "Game.h"
#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/AnimationComponent.h"
#include "../Systems/MovementSystem.h"
#include "../Systems/RenderSystem.h"
#include "../Systems/AnimationSystem.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define DEBUG

Game::Game() {
	isRunning = false;
	Logger::Log("Game constructor called");
	registry = std::make_unique<Registry>();
	assetStore = std::make_unique<AssetStore>();
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
	// Better to not scale the window to the users display.
	// Instead set window mode to fullscreen and let SDL scale the fixed window to that size
	// The difference is if we scale the window users with higher width and height displays will see more of the game
	// because we will be trying to fill all those pixels with game data
	// 
	// If we keep window size fixed and let sdl scale it then all resolutions will see the same size
#ifdef DEBUG
	windowWidth = WINDOW_WIDTH; 
	windowHeight = WINDOW_HEIGHT;
	// Not using refresh rate at the moment. Instead in debug mode we render at 60 fps fixed. In non debug uncapped
	refreshRate = 60;
#else
	SDL_DisplayMode displayMode;
	if (SDL_GetCurrentDisplayMode(0, &displayMode) != 0) {
		Logger::Err("Error getting SDL display mode");
		Logger::Err(SDL_GetError());
	}
	windowWidth = displayMode.w;
	windowHeight = displayMode.h;
	refreshRate = displayMode.refresh_rate;
#endif

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

#ifndef DEBUG
	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
#endif

	// https://wiki.libsdl.org/SDL2/SDL_CreateRenderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer) {
		Logger::Err("Error Creating SDL Renderer");
		Logger::Err(SDL_GetError());
		return;
	}
	// Set the renderer's draw color
	// If this is commented out this means its set in the render function as its changing frame by frame
	SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);

	isRunning = true;
}

void Game::Destroy() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Game::LoadLevel(int level) {
	// Add Assets
	//assetStore->AddTexture(renderer, "tank-tiger-right", "./assets/images/tank-tiger-right.png");
	//assetStore->AddTexture(renderer, "truck-ford-right", "./assets/images/truck-ford-right.png");
	assetStore->AddTexture(renderer, "tilemap-image", "./assets/tilemaps/jungle.png");
	assetStore->AddTexture(renderer, "knight", "./assets/spritesheets/fire_knight_SpriteSheet_288x128.png");

	// Load the tilemap
	int tileSize = 32;
	double tileScale = 1.0;
	int mapNumCols = 25;
	int mapNumRows = 20;

	std::fstream mapFile;
	mapFile.open("./assets/tilemaps/jungle.map");

	for (int y = 0; y < mapNumRows; y++) {
		for (int x = 0; x < mapNumCols; x++) {
			char ch;
			mapFile.get(ch);
			int srcRectY = std::atoi(&ch) * tileSize;
			mapFile.get(ch);
			int srcRectX = std::atoi(&ch) * tileSize;
			mapFile.ignore();

			Entity tile = registry->CreateEntity();
			tile.AddComponent<TransformComponent>(glm::vec2(x * tileScale * tileSize, y * tileScale * tileSize), glm::vec2(tileScale, tileScale), 0);
			tile.AddComponent<SpriteComponent>("tilemap-image", tileSize, tileSize, srcRectX, srcRectY, 0);
		}
	}

	mapFile.close();


	// Add the systems that need to be processed in our game
	registry->AddSystem<MovementSystem>();
	registry->AddSystem<RenderSystem>();
	registry->AddSystem<AnimationSystem>();

	auto knight = registry->CreateEntity();
	knight.AddComponent<TransformComponent>(glm::vec2(0.0, 0.0), glm::vec2(1.5, 1.5));
	knight.AddComponent<SpriteComponent>("knight", 288, 128, 0, 128);
	knight.AddComponent<AnimationComponent>(8, 1);

	// Entity tank = registry->CreateEntity();
	/* Can also do it this way
	registry->AddComponent<TransformComponent>(
		tank,
		glm::vec2(10.0, 30.0),
		glm::vec2(1.0, 1.0),
		0.0);

	registry->AddComponent<RigidBodyComponent>(tank);
	*/
	/**/
	//tank.AddComponent<TransformComponent>(glm::vec2(10.0, 10.0), glm::vec2(1.0, 1.0), 0.0);
	//tank.AddComponent<RigidBodyComponent>(glm::vec2(40.0, 0.0));
	//tank.AddComponent<SpriteComponent>("tank-tiger-right", 32, 32, 0, 0, 1);

	//Entity truck = registry->CreateEntity();
	//registry->AddComponent<TransformComponent>(truck);
	//truck.AddComponent<TransformComponent>(glm::vec2(2.0, 10.0));
	//truck.AddComponent<RigidBodyComponent>(glm::vec2(2.0, 10.0));
	//truck.AddComponent<SpriteComponent>("truck-ford-right", 32, 32, 0, 0, 1);
	//truck.RemoveComponent<TransformComponent>();
}

void Game::Setup() {
	LoadLevel(1);
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
#ifdef DEBUG
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
#endif

	// Number of seconds elapsed since the last frame
	double deltaTime = (SDL_GetTicks() - millisecsPreviousFrame) / 1000.f;

	// Store the current frame time
	millisecsPreviousFrame = SDL_GetTicks();

	// Ask all simulation systems to update
	registry->GetSystem<MovementSystem>().Update(deltaTime);
	registry->GetSystem<AnimationSystem>().Update(deltaTime);
	
	// Update the entities in the registry
	registry->Update();
}


void Game::Render() {
	// If this is commented out this is now being set once in the init function since it shouldn't change frame by frame
	//SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);

	// It's recommended to clear the rederer before redrawing the current frame
	SDL_RenderClear(renderer);

	// Ask all the render system to render
	registry->GetSystem<RenderSystem>().Render(renderer, assetStore);
	
	// TODO: Render game objects.. 
	SDL_RenderPresent(renderer);
}
