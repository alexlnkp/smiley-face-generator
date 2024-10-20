#ifndef   __RANDOM_H__
#define   __RANDOM_H__

double random_uniform(void) {
    return (double)rand() / (double)RAND_MAX;
}

/* based on the math from this post: https://stackoverflow.com/a/50746409/25145880
 * under this license: https://creativecommons.org/licenses/by-sa/4.0/
 */
void randomPointInCircle(int radius, int x0, int y0, int *x, int *y) {
    double r = (double)radius * sqrt(random_uniform());
    double theta = random_uniform() * 2.0 * M_PI;

    *x = (int)((double)x0 + r * cos(theta));
    *y = (int)((double)y0 + r * sin(theta));
}

#endif