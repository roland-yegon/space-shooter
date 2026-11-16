#include <SDL3/SDL.h>
#include <stdbool.h>
#include <stdlib.h>

int main(void)
{

    int win_width = 800;
    int win_height = 600;
    
    float sprite_length = 60.0f;
    float sprite_x_pos = 100.0f;
    float sprite_y_pos = 100.0f;

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error while initializing: %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Window *window = NULL;
    window = SDL_CreateWindow("SPACE SHOOTER", win_width, win_height, SDL_WINDOW_RESIZABLE);
    
    if (window == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error while creating window: %s", SDL_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }

    SDL_Renderer *renderer = NULL;
    renderer = SDL_CreateRenderer(window, NULL);

    if (renderer == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error on initialization: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    bool running = true;
    SDL_FRect sprite = {sprite_x_pos, sprite_y_pos, sprite_length, sprite_length};

    float speed = 20000.0f;
    Uint64 previousTime = SDL_GetTicks();

    while(running)
    {
        Uint64 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - previousTime) / 1000.0f;
        previousTime = currentTime;

        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_EVENT_QUIT:
                    SDL_Log("Session terminated by user");
                    running = false;
                    break;

                case SDL_EVENT_KEY_DOWN:
                    switch (event.key.key)
                    {
                        case SDLK_ESCAPE:
                            SDL_Log("Escape key pressed. Exiting...");
                            running = false;
                            break;
                        
                        case SDLK_RIGHT:
                            sprite.x += speed * deltaTime;
                            break;
                        
                        case SDLK_LEFT:
                            sprite.x -= speed * deltaTime;
                            break;
                        
                        case SDLK_UP:
                            sprite.y -= speed * deltaTime;
                            break;
                        
                        case SDLK_DOWN:
                            sprite.y += speed * deltaTime;
                            break;
                    }
            }
        }

        SDL_SetRenderDrawColor(renderer, 127, 0, 255, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &sprite);

        SDL_RenderPresent(renderer);

    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}
