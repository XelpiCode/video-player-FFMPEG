#include <iostream>
#include <SDL3/SDL.h>

struct SDLState {
    SDL_Window* window;
    SDL_Renderer* renderer;
    int width, height;
};

bool SDL_Initialize(SDLState &state);
void cleanup(const SDLState &state);

int main() {
    SDLState state{};
    state.width = 640;
    state.height = 480;

    if (!SDL_Initialize(state)) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Failed to initialize",nullptr);
        return 1;
    };

    uint64_t prevTime = SDL_GetTicks();
    bool running = true;
    while (running) {
        uint64_t nowTime = SDL_GetTicks();
        float deltaTime = (nowTime - prevTime) / 1000.0f;

        SDL_Event event{ 0 };
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_QUIT: {
                    running = false;
                    break;
                }
                default: {
                    continue;
                }
            }
        }

        SDL_SetRenderDrawColor(state.renderer, 50, 50, 50, 255);
        SDL_RenderClear(state.renderer);

        // Do stuff here

        SDL_RenderPresent(state.renderer);
        prevTime = nowTime;
    };

    cleanup(state);
    return 0;
}

bool SDL_Initialize(SDLState &state) {
    bool InitSuccess = true;

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Error initializing SDL3", nullptr);
        InitSuccess = false;
    }

    state.window = SDL_CreateWindow("Teto....", state.width, state.height, SDL_WINDOW_RESIZABLE);
    if (!state.window) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Failed to create window", nullptr);
        InitSuccess = false;
    }

    state.renderer = SDL_CreateRenderer(state.window, nullptr);
    if (!state.renderer) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Failed to create renderer", nullptr);
        InitSuccess = false;
    }

    return InitSuccess;
}

void cleanup(const SDLState &state) {
    SDL_DestroyRenderer(state.renderer);
    SDL_DestroyWindow(state.window);
    SDL_Quit();
}