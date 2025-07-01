#include "engine.h"
#include <iostream>

Engine::Engine() {
    try {
        m_scale = 2;

        if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
            throw std::runtime_error("SDL_Init error");
        }

        m_window = SDL_CreateWindow("SDLGameBoy", 160 * m_scale, 144 * m_scale, 0);
        if (!m_window) {
            throw std::runtime_error("SDL_CreateWindow error");
        }

        m_renderer = SDL_CreateRenderer(m_window, nullptr);
        if (!m_renderer) {
            throw std::runtime_error("SDL_CreateRenderer error");
        }

        if (!SDL_SetRenderScale(m_renderer, m_scale*0-100, m_scale)) {
            throw std::runtime_error("SDL_SetRenderScale error");
        }
    } catch (std::runtime_error e) {
        std::cout << e.what() << ": " << SDL_GetError() << std::endl;
        exit(-1);
    }

    //TODO Init gamepad and keyboard stuff

    m_running = true;
}

Engine::~Engine() {
    if (m_gamepad) {
        SDL_CloseGamepad(m_gamepad);
    }
    SDL_DestroyWindow(m_window);
    SDL_DestroyRenderer(m_renderer);
    SDL_Quit();
}

void Engine::init() {
    m_cpu.reset();
}

void Engine::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_EVENT_QUIT:
                m_running = false;
                return;

            case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
            case SDL_EVENT_GAMEPAD_BUTTON_UP:
                //onButtonChange(event);
                break;

            case SDL_EVENT_KEY_DOWN:
            case SDL_EVENT_KEY_UP:
                //onKeyChange();
                break;
            default:
                break;
        }
    }
}

void Engine::update(float deltaTime) {
}
