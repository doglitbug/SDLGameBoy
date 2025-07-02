#include "engine.h"
#include <iostream>

#include "cartridge.h"

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

        if (!SDL_SetRenderScale(m_renderer, m_scale, m_scale)) {
            throw std::runtime_error("SDL_SetRenderScale error");
        }
    } catch (std::runtime_error e) {
        std::cout << e.what() << ": " << SDL_GetError() << std::endl;
        exit(-1);
    }

    //TODO Init gamepad and keyboard stuff
    m_gamepad = nullptr;
    m_running = true;
    SDL_Log("Engine initialized");
}

Engine::~Engine() {
    delete(m_cartridge);
    delete(m_mmu);
    delete(m_cpu);
    if (m_gamepad) {
        SDL_CloseGamepad(m_gamepad);
    }
    SDL_DestroyWindow(m_window);
    SDL_DestroyRenderer(m_renderer);
    SDL_Quit();
    SDL_Log("Engine destroyed");
}

void Engine::init() {
    m_cpu = new cpu();
    m_mmu = new mmu();
    m_cartridge = new cartridge();

    m_cpu->reset();
    //Link MMU
    m_cpu->p_mmu = m_mmu;
    //Link cartridge
    m_mmu->p_cartridge = m_cartridge;

    //Load testing file
    m_cartridge->loadCartridgeFromFile("assets/Tetris.gb");
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
                //TODO Ctrl+O for open etc
            case SDL_EVENT_KEY_UP:
                //onKeyChange();
                break;
            default:
                break;
        }
    }
}

void Engine::update(const float deltaTime) const {
    m_cpu->tick(deltaTime);
    SDL_Delay(100);
}
