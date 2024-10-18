#ifndef   __WINDOW_H__
#define   __WINDOW_H__

typedef struct window_state_t {
    bool wants_to_quit;
} window_state_t;

typedef struct window_ctx_t {
    SDL_Window *window;
    SDL_Renderer *renderer;
} window_ctx_t;

void init_backend(void) {
    SDL_Init(SDL_INIT_VIDEO);
}

window_ctx_t* init_window_ctx(void) {
    window_ctx_t* res = malloc(sizeof(window_ctx_t));

    res->window = SDL_CreateWindow(
        "fnnuy", 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL
    );

    res->renderer = SDL_CreateRenderer(res->window, -1, 0);

    return res;
}

void deinit_backend(window_ctx_t* w_ctx) {
    SDL_DestroyRenderer(w_ctx->renderer);
    SDL_DestroyWindow(w_ctx->window);

    SDL_Quit();
}

void deinit_window_ctx(window_ctx_t** w_ctx) {
    free(*w_ctx);
}

void poll_events(SDL_Event* restrict e, window_state_t* restrict ws) {
    while (SDL_PollEvent(e)) {
        if (e->type == SDL_QUIT) {
            ws->wants_to_quit = true;
        }
        if (e->type == SDL_KEYDOWN) {
            switch (e->key.keysym.sym) {
            case SDLK_r: {
                randomPointInCircle(HALF_WW - (ELLIPSIS_LINE_WIDTH >> 1) - EYES_RADIUS, HALF_WW, HALF_WH, &i0_x, &i0_y);
                randomPointInCircle(HALF_WW - (ELLIPSIS_LINE_WIDTH >> 1) - EYES_RADIUS, HALF_WW, HALF_WH, &i1_x, &i1_y);
            } break;

            case SDLK_w: i0_y -= EYE_MOVE_SPEED; break;
            case SDLK_s: i0_y += EYE_MOVE_SPEED; break;

            case SDLK_a: i0_x -= EYE_MOVE_SPEED; break;
            case SDLK_d: i0_x += EYE_MOVE_SPEED; break;

            case SDLK_UP:    i1_y -= EYE_MOVE_SPEED; break;
            case SDLK_DOWN:  i1_y += EYE_MOVE_SPEED; break;

            case SDLK_LEFT:  i1_x -= EYE_MOVE_SPEED; break;
            case SDLK_RIGHT: i1_x += EYE_MOVE_SPEED; break;
            }
        }
    }
}

#endif /* __WINDOW_H__ */