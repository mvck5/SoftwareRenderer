#include <SDL3/SDL.h>
#include <iostream>
#include "rasteriser.h"
#include "colour.h"

constexpr int WIDTH = 1280;
constexpr int HEIGHT = 720;


int main()
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cerr << "SDL_Init failed: "
            << SDL_GetError() << '\n';
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Software Renderer",
        WIDTH,
        HEIGHT,
        SDL_WINDOW_RESIZABLE
    );

    if (!window)
    {
        std::cerr << "SDL_CreateWindow failed: "
            << SDL_GetError() << '\n';

        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);

    if (!renderer)
    {
        std::cerr << "SDL_CreateRenderer failed: "
            << SDL_GetError() << '\n';

        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // SDL texture that will receive our software framebuffer.
    SDL_Texture* texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        WIDTH,
        HEIGHT
    );

    if (!texture)
    {
        std::cerr << "SDL_CreateTexture failed: "
            << SDL_GetError() << '\n';

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Our actual software-renderer.
    Rasteriser rend(WIDTH,HEIGHT);


    bool running = true;

    while (running)
    {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                running = false;
            }
        }

        // --------------------------------------------------
        // SOFTWARE RENDERER
        // --------------------------------------------------

        // Clear framebuffer.
        for (int y = 0; y < rend.getHeight(); ++y)
        {
            for (int x = 0; x < rend.getWidth(); ++x)
            {
                rend.getBuffer()[y * rend.getWidth() + x] = colours::black;
            }
        }

        // Draw a red pixel.
        rend.drawPixel(100, 100, colours::red);

        // Example rectangle.
        for (int y = 200; y < 400; ++y)
        {
            for (int x = 300; x < 600; ++x)
            {
                rend.drawPixel(x, y, colours::green);
            }
        }

        // --------------------------------------------------
        // SEND FRAMEBUFFER TO SDL
        // --------------------------------------------------

        SDL_UpdateTexture(
            texture,
            nullptr,
            rend.getBuffer().data(), ///////////////////////////////////////////////////////////
            rend.getPitch()
        );

        SDL_RenderClear(renderer);

        SDL_RenderTexture(
            renderer,
            texture,
            nullptr,
            nullptr
        );

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}