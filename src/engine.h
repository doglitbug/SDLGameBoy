#pragma once

#include <SDL3/SDL.h>
#include "cpu.h"
#include "mmu.h"

class Engine {
public:
    Engine();
    ~Engine();

    void init();

    void handleEvents();
    void update(float deltaTime);
    bool running() const { return m_running; }

private:
    SDL_Window *m_window;
    SDL_Renderer *m_renderer;

    float m_scale;
    bool m_running;

    //Gamepads
    SDL_Gamepad *m_gamepad;
    //std::vector<bool> m_buttonStates;
    //void onButtonChange(const SDL_Event &event);

    //Emulators (parts at the moment but these should be contained under inherited game-engines)
    cpu m_cpu;
    mmu m_mmu;
};
