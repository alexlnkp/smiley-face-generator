#include <math.h>
#include <time.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 1024

#define HALF_WW (WINDOW_WIDTH >> 1)
#define HALF_WH (WINDOW_HEIGHT >> 1)

#define ELLIPSIS_LINE_WIDTH 6
#define EYES_RADIUS 24
#define SMILE_RADIUS_DELTA 50

#define EYE_MOVE_SPEED 10

#define M_PI 3.14159265358979323846

int i0_x, i0_y; // todo: fix this ugliness somehow
int i1_x, i1_y; // todo: fix this ugliness somehow

#include "random.h"
#include "window.h"
#include "render.h"

int main(void) {
    init_backend();

    srand(time(NULL));
    randomPointInCircle(HALF_WW - (ELLIPSIS_LINE_WIDTH >> 1) - EYES_RADIUS, HALF_WW, HALF_WH, &i0_x, &i0_y);
    randomPointInCircle(HALF_WW - (ELLIPSIS_LINE_WIDTH >> 1) - EYES_RADIUS, HALF_WW, HALF_WH, &i1_x, &i1_y);

    window_ctx_t* w_ctx = init_window_ctx();

    window_state_t ws = {
        .wants_to_quit = false
    };
    
    SDL_Event e;
    while (!ws.wants_to_quit) {
        poll_events(&e, &ws, w_ctx);

        render_everything(w_ctx);
    }

    deinit_backend(w_ctx);
    deinit_window_ctx(&w_ctx);

    return 0;
}