#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <stdbool.h>
#include <stdlib.h>

#define WIN_WIDTH     800
#define WIN_HEIGHT    600
#define SPRITE_SIZE   50.0f
#define SPRITE_SPEED  300.0f

int main(void)
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *background = NULL;

    MIX_Mixer *mixer = NULL;
    MIX_Audio *music = NULL;
    MIX_Track *track = NULL;

    int status = EXIT_FAILURE;
    bool running = true;

    SDL_FRect sprite = {
        (WIN_WIDTH - SPRITE_SIZE) / 2.0f,
        WIN_HEIGHT - (SPRITE_SIZE + 2.0f),
        SPRITE_SIZE,
        SPRITE_SIZE
    };

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error during initialization: %s", SDL_GetError());
        goto cleanup;
    }

    if (!MIX_Init()) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error while initializing mixer: %s", SDL_GetError());
        goto cleanup;
    }

    window = SDL_CreateWindow("SPACE SHOOTER", WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_RESIZABLE);
    if (window == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error while creating the window: %s", SDL_GetError());
        goto cleanup;
    }

    renderer = SDL_CreateRenderer(window, NULL);
    if (renderer == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error while creating the renderer: %s", SDL_GetError());
        goto cleanup;
    }

    SDL_SetRenderVSync(renderer, 1);

    background = IMG_LoadTexture(renderer, "static/images/background_photo.png");
    if (background == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error while loading the background: %s", SDL_GetError());
        goto cleanup;
    }

    mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
    if (mixer == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error while creating the mixer: %s", SDL_GetError());
        goto cleanup;
    }

    music = MIX_LoadAudio(mixer, "static/music/background_music.mp3", false);
    if (music == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error while loading the music: %s", SDL_GetError());
        goto cleanup;
    }

    track = MIX_CreateTrack(mixer);
    if (track == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error while creating the track: %s", SDL_GetError());
        goto cleanup;
    }

    if (!MIX_SetTrackAudio(track, music)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error while assigning audio to track: %s", SDL_GetError());
        goto cleanup;
    }

    {
        SDL_PropertiesID options = SDL_CreateProperties();
        SDL_SetNumberProperty(options, MIX_PROP_PLAY_LOOPS_NUMBER, -1);

        if (!MIX_PlayTrack(track, options)) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error while playing the music: %s", SDL_GetError());
        }

        SDL_DestroyProperties(options);
    }

    Uint64 previous_time = SDL_GetTicks();

    while (running) {
        Uint64 current_time = SDL_GetTicks();
        float delta_time = (current_time - previous_time) / 1000.0f;
        previous_time = current_time;

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_EVENT_QUIT:
                SDL_Log("Session terminated by user");
                running = false;
                break;

            case SDL_EVENT_KEY_DOWN:
                if (event.key.key == SDLK_ESCAPE) {
                    SDL_Log("Escape key pressed. Exiting...");
                    running = false;
                }
                break;

            default:
                break;
            }
        }

        const bool *keys = SDL_GetKeyboardState(NULL);

        if (keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_D])
            sprite.x += SPRITE_SPEED * delta_time;
        if (keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_A])
            sprite.x -= SPRITE_SPEED * delta_time;
        if (keys[SDL_SCANCODE_UP] || keys[SDL_SCANCODE_W])
            sprite.y -= SPRITE_SPEED * delta_time;
        if (keys[SDL_SCANCODE_DOWN] || keys[SDL_SCANCODE_S])
            sprite.y += SPRITE_SPEED * delta_time;

        /* Keep the sprite inside the window, even after resizing */
        int win_w = 0, win_h = 0;
        SDL_GetWindowSize(window, &win_w, &win_h);

        if (sprite.x < 0.0f)
            sprite.x = 0.0f;
        if (sprite.y < 0.0f)
            sprite.y = 0.0f;
        if (sprite.x > win_w - sprite.w)
            sprite.x = win_w - sprite.w;
        if (sprite.y > win_h - sprite.h)
            sprite.y = win_h - sprite.h;

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_RenderTexture(renderer, background, NULL, NULL);

        SDL_SetRenderDrawColor(renderer, 255, 0, 127, 255);
        SDL_RenderFillRect(renderer, &sprite);

        SDL_RenderPresent(renderer);
    }

    status = EXIT_SUCCESS;

cleanup:
    /* Destroy in reverse order of creation */
    if (track != NULL)
        MIX_DestroyTrack(track);

    if (music != NULL)
        MIX_DestroyAudio(music);

    if (mixer != NULL)
        MIX_DestroyMixer(mixer);

    if (background != NULL)
        SDL_DestroyTexture(background);

    if (renderer != NULL)
        SDL_DestroyRenderer(renderer);

    if (window != NULL)
        SDL_DestroyWindow(window);

    MIX_Quit();
    SDL_Quit();
    return status;
}
