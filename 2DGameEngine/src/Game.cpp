#include "Game.h"
#include <iostream>
#include <SDL_image.h>

Game::Game() {
    isRunning = false;
    frame = 0;
    std::cout << "Game constructor called" << std::endl;
}

Game::~Game() {
    std::cout << "Game destructor called" << std::endl;
}

void Game::Initialize() {
    if (SDL_Init(SDL_INIT_EVERYTHING)) {
        std::cerr << "Error initializing SDL" << std::endl;
        std::cerr << SDL_GetError() << std::endl;
        return;            
    }

    SDL_DisplayMode displayMode;
    if (SDL_GetCurrentDisplayMode(0, &displayMode) != 0) {
        std::cerr << "Error getting SDL display mode" << std::endl;
        std::cerr << SDL_GetError() << std::endl;

    }
    // Better to not scale the window to the users display.
    // Instead set window mode to fullscreen and let SDL scale the fixed window to that size
    // The difference is if we scale the window users with higher width and height displays will see more of the game
    // because we will be trying to fill all those pixels with game data
    // 
    // If we keep window size fixed and let sdl scale it then all resolutions will see the same size
    windowWidth = 800; //displayMode.w;
    windowHeight = 600; //displayMode.h;

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
        std::cerr << "Error creating SDL window" << std::endl;
        std::cerr << SDL_GetError() << std::endl;
        return;
    }
    
    // Let SDL set the displayMode to FULLSCREEN
    // and scale our window to be the size of the screen (as big as possible while preserving aspect ratio)
    // Is this the same as setting SDL_CreateWindow flag to SDL_WINDOW_FULLSCREEN
    
    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

    // https://wiki.libsdl.org/SDL2/SDL_CreateRenderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        std::cerr << "Error Creating SDL Renderer" << std::endl;
        std::cerr << SDL_GetError() << std::endl;
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

}

void Game::Run() {
    Setup();
    while (isRunning) {
        ProcessInput();
        Update();
        Render();
        frame = (frame + 1) % 8;
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
                //std::cout << "We got a mouse event" << std::endl << " Current mouse position " <<
                    //event.motion.x << event.motion.y << std::endl;
            break;
            case SDL_KEYDOWN:
                std::cout << "We got a key pressed" << std::endl << " Current key pressed: " << 
                    event.key.keysym.sym << std::endl;
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    isRunning = false;
                }
            break;
            case SDL_KEYUP:
                std::cout << "We got a key released" << std::endl << " Current key pressed: " <<
                    event.key.keysym.sym << std::endl;
            break;
            /*default:
                std::cout << "Unhandled event" << std::endl;
                std::cout << event.type << std::endl;
            break;*/
        }
    }
}

void Game::Update() {

}


void Game::Render() {
    // This is now being set once in the init function since it shouldn't change frame by frame
    // SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
    
    // It's recommended to clear the rederer before redrawing the current frame
    SDL_RenderClear(renderer);
   
    SDL_Surface* surface = IMG_Load("./assets/spritesheets/fire_knight_SpriteSheet_288x128.png");
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    // Can free the surface if not needed again
    //SDL_FreeSurface(surface);
    
    SDL_Rect destRect = { 0, 0, 720, 320 };
    SDL_Rect srcRect = { 288 * frame, 128 * 5 , 288, 128 };
    SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
    
    
    //SDL_DestroyTexture(texture);

    SDL_RenderPresent(renderer);
}
