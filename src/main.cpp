#include <SDL3/SDL.h>
#include <iostream>
#include "rasteriser.h"
#include "colour.h"
#include "maths.h"
#include "object.h"

constexpr int WIDTH = 1280;
constexpr int HEIGHT = 720;

std::vector<Maths::Vec3f> squareVertices{
    Maths::Vec3f{-0.5f,-0.5f, 0.0f}, //bottom left
    Maths::Vec3f{ 0.5f,-0.5f, 0.0f}, //bottom right
    Maths::Vec3f{-0.5f, 0.5f, 0.0f}, //top left
    Maths::Vec3f{ 0.5f, 0.5f, 0.0f}  //top right

};
std::vector<std::array<int, 3>> squareDrawOrder{ 
    std::array<int, 3>{0,1,2},
    std::array<int, 3>{1,2,3}
};

std::vector<Maths::Vec3f> cubeVertices{
    Maths::Vec3f{-0.5f,-0.5f,-0.5f}, //back bottom left
    Maths::Vec3f{ 0.5f,-0.5f,-0.5f}, //back bottom right
    Maths::Vec3f{-0.5f, 0.5f,-0.5f}, //back top left
    Maths::Vec3f{ 0.5f, 0.5f,-0.5f}, //back top right

    Maths::Vec3f{-0.5f,-0.5f, 0.5f}, //front bottom left
    Maths::Vec3f{ 0.5f,-0.5f, 0.5f}, //front bottom right
    Maths::Vec3f{-0.5f, 0.5f, 0.5f}, //front top left
    Maths::Vec3f{ 0.5f, 0.5f, 0.5f} //front top right
};
std::vector<std::array<int, 3>> cubeDrawOrder{ 
    std::array<int, 3>{0,1,2}, //front face
    std::array<int, 3>{1,2,3},

    std::array<int, 3>{4,5,6}, //back face
    std::array<int, 3>{5,6,7},

    std::array<int, 3>{0,2,4}, //left face
    std::array<int, 3>{2,4,6},

    std::array<int, 3>{1,3,5}, //right face
    std::array<int, 3>{3,5,7},

    std::array<int, 3>{2,3,6}, //top face
    std::array<int, 3>{3,6,7},

    std::array<int, 3>{0,1,4}, //bottom face
    std::array<int, 3>{1,4,5}
};



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

    // sample object
    Object square1{ squareVertices,squareDrawOrder };
    square1.move(Maths::Vec3f{ 2.0f,0.0f,-4.0f }); 

    Object square2{ squareVertices,squareDrawOrder };
    square2.move(Maths::Vec3f{ -2.0f,0.0f,-4.0f }); 
    
    Object cube{ cubeVertices,cubeDrawOrder };
    cube.move(Maths::Vec3f{ 0.0f,0.0f,-4.0f });

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
                rend.getFrameBuffer()[y * rend.getWidth() + x] = Colours::black;
            }
        }

        // Clear z buffer
        for (int y = 0; y < rend.getHeight(); ++y)
        {
            for (int x = 0; x < rend.getWidth(); ++x)
            {
                rend.getZBuffer()[y * rend.getWidth() + x] = 10000000.0f; //could maybe use some numeric_limit to make sure it's the max value
            }
        }

        // Draw the objects 
        rend.drawObject(square1);
        square1.rotate(Maths::degreeToRadian(1.0f), Maths::Vec3f{ 0.0f,1.0f,0.0f });

        rend.drawObject(square2);
        square2.rotate(Maths::degreeToRadian(1.0f), Maths::Vec3f{ 1.0f,0.0f,0.0f });

        rend.drawObject(cube);
        cube.rotate(Maths::degreeToRadian(1.0f), Maths::Vec3f{ 1.0f,1.0f,0.0f });

        // --------------------------------------------------
        // SEND FRAMEBUFFER TO SDL
        // --------------------------------------------------

        SDL_UpdateTexture(
            texture,
            nullptr,
            rend.getFrameBuffer().data(), ///////////////////////////////////////////////////////////
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