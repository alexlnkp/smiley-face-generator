#ifndef   __RENDER_H__
#define   __RENDER_H__

#include <SDL2/SDL2_gfxPrimitives.h>

/* based on this function: https://stackoverflow.com/a/47766167/25145880 */
void SDL_RenderHollowEllipse(SDL_Renderer* r, int x0, int y0, int radiusX, int radiusY) {
    float pih = M_PI / 2.0; // half of pi

    const int prec = 54; // precision value; value of 1 will draw a diamond, 27 makes pretty smooth circles.
    float theta = 0;     // angle that will be increased each loop

    // starting point
    int x  = (float)radiusX * cos(theta); // start point
    int y  = (float)radiusY * sin(theta); // start point
    int x1 = x;
    int y1 = y;

    // repeat until theta >= 90;
    float step = pih / (float)prec; // amount to add to theta each time (degrees)

    for (theta = step; theta <= pih; theta += step) { // step through only a 90 arc (1 quadrant)
        // get new point location
        x1 = (float)radiusX * cosf(theta) + 0.5; // new point (+.5 is a quick rounding method)
        y1 = (float)radiusY * sinf(theta) + 0.5; // new point (+.5 is a quick rounding method)

        // draw line from previous point to new point, ONLY if point incremented
        if ((x != x1) || (y != y1)) {
            thickLineRGBA(r, x0 + x, y0 - y, x0 + x1, y0 - y1, ELLIPSIS_LINE_WIDTH, 0, 0, 0, 255);
            thickLineRGBA(r, x0 - x, y0 - y, x0 - x1, y0 - y1, ELLIPSIS_LINE_WIDTH, 0, 0, 0, 255);
            thickLineRGBA(r, x0 - x, y0 + y, x0 - x1, y0 + y1, ELLIPSIS_LINE_WIDTH, 0, 0, 0, 255);
            thickLineRGBA(r, x0 + x, y0 + y, x0 + x1, y0 + y1, ELLIPSIS_LINE_WIDTH, 0, 0, 0, 255);
        }

        x = x1; // save new previous point
        y = y1; // save new previous point
    }

    // arc did not finish because of rounding, so finish the arc
    if (x != 0) {
        x = 0;

        thickLineRGBA(r, x0 + x, y0 - y, x0 + x1, y0 - y1, ELLIPSIS_LINE_WIDTH, 0, 0, 0, 255);
        thickLineRGBA(r, x0 - x, y0 - y, x0 - x1, y0 - y1, ELLIPSIS_LINE_WIDTH, 0, 0, 0, 255);
        thickLineRGBA(r, x0 - x, y0 + y, x0 - x1, y0 + y1, ELLIPSIS_LINE_WIDTH, 0, 0, 0, 255);
        thickLineRGBA(r, x0 + x, y0 + y, x0 + x1, y0 + y1, ELLIPSIS_LINE_WIDTH, 0, 0, 0, 255);
    }
}

void SDL_RenderFilledEllipse(SDL_Renderer* r, int x0, int y0, int radiusX, int radiusY) {
    float pih = M_PI / 2.0; // half of pi

    const int prec = 54; // precision value; value of 1 will draw a diamond, 27 makes pretty smooth circles.
    float theta = 0;     // angle that will be increased each loop

    // repeat until theta >= 90;
    float step = pih / (float)prec; // amount to add to theta each time (degrees)

    for (theta = 0; theta <= pih; theta += step) { // step through only a 90 arc (1 quadrant)
        // get new point location
        int x1 = (float)radiusX * cosf(theta) + 0.5; // new point (+.5 is a quick rounding method)
        int y1 = (float)radiusY * sinf(theta) + 0.5; // new point (+.5 is a quick rounding method)

        // draw horizontal lines from -x1 to +x1 at y0 - y1 and y0 + y1
        for (int y = -y1; y <= y1; y++) {
            thickLineRGBA(r, x0 - x1, y0 + y, x0 + x1, y0 + y, ELLIPSIS_LINE_WIDTH, 0, 0, 0, 255);
        }
    }
}

void SDL_RenderArc(SDL_Renderer* r, int centerX, int centerY, int radius, float startAngle, float endAngle) {
    const int prec = 54; // precision value
    float angleStep = (endAngle - startAngle) / prec; // step size for the angle

    for (int i = 0; i < prec; i++) {
        float theta1 = startAngle + i * angleStep;
        float theta2 = startAngle + (i + 1) * angleStep;

        int x1 = (int)((double)centerX + (double)radius * cos(theta1));
        int y1 = (int)((double)centerY + (double)radius * sin(theta1));
        int x2 = (int)((double)centerX + (double)radius * cos(theta2));
        int y2 = (int)((double)centerY + (double)radius * sin(theta2));

        // draw line segment between (x1, y1) and (x2, y2)
        thickLineRGBA(r, x1, y1, x2, y2, ELLIPSIS_LINE_WIDTH, 0, 0, 0, 255);
    }
}

void drawArcBetweenPoints(SDL_Renderer* r, int centerX, int centerY, int radius, int x1, int y1, int x2, int y2) {
    /*previous definition
    // calculate angles for the two points
    float startAngle = atan2(y1 - centerY, x1 - centerX);
    float endAngle = atan2(y2 - centerY, x2 - centerX);

    // ensure the angles are in the correct order
    if (endAngle < startAngle) {
        endAngle += 2 * M_PI; // wrap around if necessary
    }

    // draw the arc
    SDL_RenderArc(r, centerX, centerY, radius, startAngle, endAngle);
    *********************/

    // calculate theta for both points relative to the origin (0,0)
    float angle1 = atan2(y1, x1); // angle for point (x1, y1)
    float angle2 = atan2(y2, x2); // angle for point (x2, y2)

    // determine which point has the highest theta
    float startAngle, endAngle;
    if (angle1 < angle2) {
        startAngle = atan2(y1 - centerY, x1 - centerX); // angle for point (x1, y1)
        endAngle = atan2(y2 - centerY, x2 - centerX);   // angle for point (x2, y2)
    } else {
        startAngle = atan2(y2 - centerY, x2 - centerX); // angle for point (x2, y2)
        endAngle = atan2(y1 - centerY, x1 - centerX);   // angle for point (x1, y1)
    }

    // ernsure the angles are in the correct order (maybe unnecessary due to the top calc)
    // if (endAngle < startAngle) {
    //     endAngle += 2 * M_PI; // wrap around if necessary
    // }

    // draw the arc
    SDL_RenderArc(r, centerX, centerY, radius, startAngle, endAngle);
}

void render_everything(window_ctx_t* w_ctx) {
    SDL_SetRenderDrawColor(w_ctx->renderer, 255, 255, 255, 255);

    SDL_RenderClear(w_ctx->renderer); {
        SDL_SetRenderDrawColor(w_ctx->renderer, 0, 0, 0, 255);
        SDL_RenderHollowEllipse(w_ctx->renderer, HALF_WW, HALF_WH, HALF_WW - (ELLIPSIS_LINE_WIDTH >> 1), HALF_WH - (ELLIPSIS_LINE_WIDTH >> 1));
        
        SDL_RenderFilledEllipse(w_ctx->renderer, i0_x, i0_y, EYES_RADIUS, EYES_RADIUS);
        SDL_RenderFilledEllipse(w_ctx->renderer, i1_x, i1_y, EYES_RADIUS, EYES_RADIUS);

        drawArcBetweenPoints(w_ctx->renderer, HALF_WW, HALF_WH, HALF_WW - (ELLIPSIS_LINE_WIDTH >> 1) - SMILE_RADIUS_DELTA, i0_x, i0_y, i1_x, i1_y);

    } SDL_RenderPresent(w_ctx->renderer);
}

#endif /* __RENDER_H__ */