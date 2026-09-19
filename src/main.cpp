#include <SDL3/SDL.h>
#include <iostream>
#include "rasteriser.h"
#include "colour.h"
#include "maths.h"
#include "object.h"
#include "timer.h"
#include "mesh.h"
#include "texture.h"

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
    
    // Load a mesh
    Mesh cubeMesh("cube.obj");

    // Load a texture
    Texture crateTex("container.jpg");

    //create an object
    Object cube(cubeMesh,crateTex);
    cube.move(Maths::Vec3f{ 0.0f,0.0f,-5.0f });


    
    //frame rate stuff
    Timer timer{};
    std::array<float, 20> times{};
    int index{ 0 };
    float frameAverage{ 0.0f };

    //movement stuff
    const bool* key_states = SDL_GetKeyboardState(NULL);
    float speed{ 0.1 };

    bool running = true;

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
            rend.getCamera().move(Maths::Vec3f{0.0f,0.0f,-speed });
        }
        if (key_states[SDL_SCANCODE_A]) {
            rend.getCamera().move(Maths::Vec3f{ -speed,0.0f,0.0f });
        }
        if (key_states[SDL_SCANCODE_S]) {
            rend.getCamera().move(Maths::Vec3f{ 0.0f,0.0f,speed });
        }
        if (key_states[SDL_SCANCODE_D]) {
            rend.getCamera().move(Maths::Vec3f{ speed,0.0f,0.0f });
        }
        if (key_states[SDL_SCANCODE_Q]) {
            rend.getCamera().move(Maths::Vec3f{ 0.0f,speed,0.0f });
        }
        if (key_states[SDL_SCANCODE_E]) {
            rend.getCamera().move(Maths::Vec3f{ 0.0f,-speed,0.0f });
        }
        if (key_states[SDL_SCANCODE_T]) {
            rend.getCamera().rotate(Maths::degreeToRadian(1.0f), Maths::Vec3f{ speed,0.0f,0.0f });
        }
        if (key_states[SDL_SCANCODE_G]) {
            rend.getCamera().rotate(Maths::degreeToRadian(1.0f), Maths::Vec3f{ -speed,0.0f,0.0f });
        }
        if (key_states[SDL_SCANCODE_F]) {
            rend.getCamera().rotate(Maths::degreeToRadian(1.0f), Maths::Vec3f{ 0.0f,speed,0.0f });
        }
        if (key_states[SDL_SCANCODE_H]) {
            rend.getCamera().rotate(Maths::degreeToRadian(1.0f), Maths::Vec3f{ 0.0f,-speed,0.0f });
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
        rend.drawObject(cube);
        cube.rotate(Maths::degreeToRadian(0.1f), Maths::Vec3f{0.2f,0.2f,0.2f});
       

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