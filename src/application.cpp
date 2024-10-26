#include "application.h"

#include <imgui.h>
#include <iostream>

#include "boids.h"
#include "imgui-1.91.4/backends/imgui_impl_sdl3.h"
#include "imgui-1.91.4/backends/imgui_impl_sdlrenderer3.h"
#include "SDL3/SDL.h"

namespace boids {
    SDL_Window *g_window = nullptr;
    SDL_Renderer *g_sdlRenderer = nullptr;
    ImGuiIO *g_io;
    Boids *g_boids;
    int windowSizeX, windowSizeY;

    void InitImgui() {

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        g_io = &ImGui::GetIO(); (void)*g_io;
        g_io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        g_io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsLight();

        // Setup Platform/Renderer backends
        ImGui_ImplSDL3_InitForSDLRenderer(g_window, g_sdlRenderer);
        ImGui_ImplSDLRenderer3_Init(g_sdlRenderer);
    }

    void ImguiRender(Boids *boids) {
        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Settings");
        ImGui::Text("Boids count: %d", boids->boids.size());
        ImGui::Text("FPS: %.1f", g_io->Framerate);

        ImGui::InputFloat("Separation radius", &boids->separationRadius);
        ImGui::InputFloat("Separation weight", &boids->separationWeight, 0, 0, "%.8f");
        ImGui::InputFloat("Alignment radius", &boids->alignmentRadius);
        ImGui::InputFloat("Alignment weight", &boids->alignmentWeight, 0, 0, "%.8f");
        ImGui::InputFloat("Cohesion radius", &boids->cohesionRadius);
        ImGui::InputFloat("Cohesion weight", &boids->cohesionWeight, 0, 0, "%.8f");
        ImGui::InputFloat("Bias radius", &boids->biasRadius);
        ImGui::InputFloat("Bias weight", &boids->biasWeight, 0, 0, "%.8f");
        ImGui::End();

        ImGui::Render();
    }

    int Application::Run() {
        bool sdl_init_result = SDL_Init(SDL_INIT_VIDEO);
        if (sdl_init_result != true) {
            // spdlog::critical("Unable to initialize SDL: {}", SDL_GetError());
            std::cerr << "Unable to initialize SDL: " << SDL_GetError() << std::endl;
            return 1;
        }

        g_window = SDL_CreateWindow("rimclone", 1280, 720,
                                    // SDL_WINDOW_FULLSCREEN | SDL_WINDOW_BORDERLESS | SDL_WINDOW_OPENGL);
                                    SDL_WINDOW_BORDERLESS);

        if (g_window == nullptr) {
            // spdlog::critical("Window could not be created! SDL_Error: {}", SDL_GetError());
            std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
            return 1;
        }
        SDL_GetWindowSize(g_window, &windowSizeX, &windowSizeY);

        g_sdlRenderer = SDL_CreateRenderer(g_window, 0);
        if (g_sdlRenderer == nullptr) {
            // spdlog::critical("Renderer could not be created! SDL_Error: {}", SDL_GetError());
            std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
            return 1;
        }
        SDL_SetRenderVSync(g_sdlRenderer, false);

        InitImgui();

        g_boids = new Boids(300, {static_cast<float>(windowSizeX), static_cast<float>(windowSizeY)}, 300, 600);

        SDL_Event e;
        bool quit = false;
        while (!quit) {
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_EVENT_QUIT) quit = true;

                // process event for ui
                ImGui_ImplSDL3_ProcessEvent(&e);
            }

            const bool *currentKeyStates = SDL_GetKeyboardState(nullptr);
            if (currentKeyStates[SDL_SCANCODE_ESCAPE]) {
                quit = true;
                break;
            }

            m_deltaTicks = SDL_GetTicks() - m_lastFrameTicks;
            m_deltaTime = static_cast<float>(m_deltaTicks) / 1000.0f;
            m_lastFrameTicks = SDL_GetTicks();

            vec2 mousePos;
            SDL_GetMouseState(&mousePos.x, &mousePos.y);

            g_boids->update(m_deltaTime, mousePos);

            ImguiRender(g_boids);

            SDL_SetRenderDrawColor(g_sdlRenderer, 0, 0, 0, 255);
            SDL_RenderClear(g_sdlRenderer);

            // SDL_SetRenderDrawColor(g_sdlRenderer, 255, 0, 0, 255);
            // SDL_FRect rect = {0, 0, 100, 100};
            // SDL_RenderRect(g_sdlRenderer, &rect);

            g_boids->render(g_sdlRenderer);
            ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), g_sdlRenderer);

            SDL_RenderPresent(g_sdlRenderer);
        }

        delete g_boids;

        // Cleanup
        ImGui_ImplSDLRenderer3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();

        SDL_DestroyRenderer(g_sdlRenderer);
        SDL_DestroyWindow(g_window);
        SDL_Quit();

        return 0;
    }
}