#ifndef GAME_H
#define GAME_H
#include <SDL.h>

class Game {
    private:
        bool isRunning;
        SDL_Window* window;
        SDL_Renderer* renderer; 

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
        // TODO(yudi): Remove this ASAP. Only to test the spritesheet animation
        int frame;
};

#endif
