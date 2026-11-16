#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <stdbool.h>
#include <stdlib.h>

int main(void)
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *background = NULL;

    int status = EXIT_FAILURE;
    int win_width = 800;
    int win_height = 600;

    float sprite_length = 50.0f;
    float sprite_width = 50.0f;

    float sprite_x_pos = (win_width - sprite_width) / 2.0f;
    float sprite_y_pos = win_height - (sprite_length + 2.0f);

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error during initialization: %s", SDL_GetError());
        goto cleanup;
    }

    window = SDL_CreateWindow("SPACE SHOOTER", win_width, win_height, SDL_WINDOW_RESIZABLE);

    if (window == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error while creating the window: %s", SDL_GetError());
        goto cleanup;
    }

    renderer = SDL_CreateRenderer(window, NULL);

    if (renderer == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error while creating the renderer: %s", SDL_GetError());
        goto cleanup;
    }

    background = IMG_LoadTexture(renderer, "static/background.png");

    if (background == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error while creating the texture: %s", SDL_GetError());
        goto cleanup;
    }

    bool running = true;
    SDL_FRect sprite = {sprite_x_pos, sprite_y_pos, sprite_width, sprite_length};

    float speed = 300.0f;
    Uint64 previousTime = SDL_GetTicks();

    while (running) {
        Uint64 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - previousTime) / 1000.0f;
        previousTime = currentTime;

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_EVENT_QUIT:
                SDL_Log("Session terminated by user");
                running = false;
                break;

            case SDL_EVENT_KEY_DOWN:
                switch (event.key.key) {
                case SDLK_ESCAPE:
                    SDL_Log("Escape key pressed. Exiting...");
                    running = false;
                    break;
                }
            }
        }

        const bool *keys = SDL_GetKeyboardState(NULL);

        if (keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_D])
            sprite.x += speed * deltaTime;
        if (keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_A])
            sprite.x -= speed * deltaTime;
        if (keys[SDL_SCANCODE_UP] || keys[SDL_SCANCODE_W])
            sprite.y -= speed * deltaTime;
        if (keys[SDL_SCANCODE_DOWN] || keys[SDL_SCANCODE_S])
            sprite.y += speed * deltaTime;

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_RenderTexture(renderer, background, NULL, NULL);

        SDL_SetRenderDrawColor(renderer, 255, 0, 127, 255);
        SDL_RenderFillRect(renderer, &sprite);

        SDL_RenderPresent(renderer);
    }

    status = EXIT_SUCCESS;

    cleanup:

        if (window != NULL)
            SDL_DestroyWindow(window);

        if (renderer != NULL)
            SDL_DestroyRenderer(renderer);

        if (background != NULL)
            SDL_DestroyTexture(background);

        SDL_Quit();
        return status;
}
