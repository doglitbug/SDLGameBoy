#include <iostream>
#include <ostream>
#include <SDL3/SDL.h>

#include "engine.h"

constexpr int FPS = 60;
constexpr int DELAY_TIME = static_cast<int>(1000.0f / FPS);

int main(int argc, char *argv[]) {
    float deltaTime = 0;

    auto *engine = new Engine();

    engine->init();//Load Wanted EmulationEngine (GB at this stage)

    while (engine->running()) {
        const Uint64 frameStart = SDL_GetTicks();

        engine->handleEvents();
        engine->update(deltaTime);

        //Limit FPS so we don't burn out the GPU (kidding...)
        if (const Uint64 frameTime = SDL_GetTicks() - frameStart; frameTime < DELAY_TIME) {
            //SDL_Delay(static_cast<int>(DELAY_TIME - frameTime));
        }

        deltaTime = (SDL_GetTicks() - frameStart) / 1000.0f;
    }

    delete engine;
    return 0;
}