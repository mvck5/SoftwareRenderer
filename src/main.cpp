#include <SDL3/SDL.h>
#include <iostream>
#include "rasteriser.h"
#include "colour.h"
#include "maths.h"
#include "object.h"
#include "timer.h"

constexpr int WIDTH = 1280;
constexpr int HEIGHT = 720;

//note that vertices should be declared counter-clockwise

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
    Maths::Vec3f{ 0.5f, 0.5f, 0.5f}  //front top right
};
std::vector<std::array<int, 3>> cubeDrawOrder{ 
    std::array<int, 3>{0,1,2}, //back face
    std::array<int, 3>{1,2,3},

    std::array<int, 3>{4,5,6}, //front face
    std::array<int, 3>{5,6,7},

    std::array<int, 3>{0,4,2}, //left face
    std::array<int, 3>{2,4,6},

    std::array<int, 3>{1,3,5}, //right face
    std::array<int, 3>{3,7,5},

    std::array<int, 3>{2,6,3}, //top face
    std::array<int, 3>{3,6,7},

    std::array<int, 3>{0,4,1}, //bottom face
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
    
    Object cube1{ cubeVertices,cubeDrawOrder };
    cube1.move(Maths::Vec3f{ 0.0f,2.0f,-4.0f });
    Object cube2{ cubeVertices,cubeDrawOrder };
    cube2.move(Maths::Vec3f{ 0.0f,0.0f,-4.0f });
    Object cube3{ cubeVertices,cubeDrawOrder };
    cube3.move(Maths::Vec3f{ 0.0f,-2.0f,-4.0f });

    bool running = true;

    Timer timer{};
    
    std::array<float, 20> times{};
    int index{ 0 };
    float frameAverage{ 0.0f };

    const bool* key_states = SDL_GetKeyboardState(NULL);

    

    while (running)
    {
        timer.start();

       // --------------------------------------------------
       // USER INPUTS
       // --------------------------------------------------

        /*
        w - move forwards 
        a - move left
        s - move down 
        d - move right
        q - move up
        e - move down
        t - look up
        g - look down
        f - look left
        h - look right
        */
        
        
        key_states = SDL_GetKeyboardState(NULL);

        if (key_states[SDL_SCANCODE_W]) {
            rend.getCamera().move(Maths::Vec3f{0.0f,0.0f,-0.1f});
        }
        if (key_states[SDL_SCANCODE_A]) {
            rend.getCamera().move(Maths::Vec3f{ -0.1f,0.0f,0.0f });
        }
        if (key_states[SDL_SCANCODE_S]) {
            rend.getCamera().move(Maths::Vec3f{ 0.0f,0.0f,0.1f });
        }
        if (key_states[SDL_SCANCODE_D]) {
            rend.getCamera().move(Maths::Vec3f{ 0.1f,0.0f,0.0f });
        }
        if (key_states[SDL_SCANCODE_Q]) {
            rend.getCamera().move(Maths::Vec3f{ 0.0f,0.1f,0.0f });
        }
        if (key_states[SDL_SCANCODE_E]) {
            rend.getCamera().move(Maths::Vec3f{ 0.0f,-0.1f,0.0f });
        }
        if (key_states[SDL_SCANCODE_T]) {
            rend.getCamera().rotate(Maths::degreeToRadian(0.5f), Maths::Vec3f{ 1.0f,0.0f,0.0f });
        }
        if (key_states[SDL_SCANCODE_G]) {
            rend.getCamera().rotate(Maths::degreeToRadian(0.5f), Maths::Vec3f{ -1.0f,0.0f,0.0f });
        }
        if (key_states[SDL_SCANCODE_F]) {
            rend.getCamera().rotate(Maths::degreeToRadian(0.5f), Maths::Vec3f{ 0.0f,1.0f,0.0f });
        }
        if (key_states[SDL_SCANCODE_H]) {
            rend.getCamera().rotate(Maths::degreeToRadian(0.5f), Maths::Vec3f{ 0.0f,-1.0f,0.0f });
        }
        
       
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
        
        rend.drawObject(cube1);
        cube1.rotate(Maths::degreeToRadian(1.0f), Maths::Vec3f{ 1.0f,0.0f,0.0f });
        rend.drawObject(cube2,true);
        cube2.rotate(Maths::degreeToRadian(1.0f), Maths::Vec3f{ 1.0f,1.0f,0.0f });
        rend.drawObject(cube3);
        cube3.rotate(Maths::degreeToRadian(1.0f), Maths::Vec3f{ 0.0f,1.0f,0.0f });

        // --------------------------------------------------
        // SENDING FRAMEBUFFER TO SDL
        // --------------------------------------------------

        SDL_UpdateTexture(
            texture,
            nullptr,
            rend.getFrameBuffer().data(),
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


        // --------------------------------------------------
        // Framerate logic
        // --------------------------------------------------
        timer.end();
        times[index] = timer.getDuration();
        index++;
 
        if (index >= times.size()) {

            index = 0;
            frameAverage = 0.0f;

            for (std::size_t i{ 0 }; i < times.size(); i++) {
                frameAverage += times[i];
            }

            frameAverage /= times.size();
        }

        SDL_SetWindowTitle(window, std::to_string(1/frameAverage).c_str() );

    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}