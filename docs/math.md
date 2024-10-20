# randomPointInCircle

For the math behind generating a random point that lays within a circle, please refer to [**this**](https://stackoverflow.com/a/50746409/25145880) Stackoverflow post, which is shared under *CC BY-SA 4.0* license.

# SDL_RenderHollowEllipse

Math behind the [code that draws a circle/ellipse](https://stackoverflow.com/a/47766167/25145880).
The original function draws only a quarter of the whole ellipse and then mirrors the quarter to get a full ellipse, "[...] reducing calls to cosf and sinf".

The function uses parametric equations of an ellipse
- $x(\theta) = a \cdot \cos(\theta)$
- $y(\theta) = b \cdot \sin(\theta)$

$a$ is the semi-major axis (x radius).

$b$ is the semi-minor axis (y radius).

$\theta$ is the parameter that varies, typically from $0$ to $2 \pi$ for a full ellipse.

## Drawing
The function draws only a quarter of a full ellipse ($\theta$ goes only from $0$ to $\frac{\pi}{2}$), and mirrors it accross axes to complete an ellipse. As mentioned prior, this is done to reduce the number of calls to `sinf` and `cosf`, this also doesn't sacrifice the overall accuracy of an ellipse.

### For-loop
The variable `theta` (which is our $\theta$ from the formula) is initialized to $0$, and is incremented in steps defined by `step`, which is defined as $\frac{\frac{\pi}{2}}{prec}$, or $\frac{\pi}{2 \cdot prec}$, where $prec$ is the precision of our ellipse.

As the precision value increases, the shape of our ellipse approaches more and more accurate depiction of self. Value of $1$ renders it as a diamond/rhombus, and value of $27$ gives a rather accurate ellipse.



The loop iterates through angles from `step` to $\frac{\pi}{2}$, calculating the $x$ and $y$ coordinates for each angle using the (float) cosine and (float) sine functions.

Since **SDL2_gfx** only works with integer coordinates, we are forced to do so as well.

The $\sin$ and $\cos$ values are going to be floored ($3.98$ becomes $3$) to become integers either way, therefore we don't need extra precision from `double` versions of $\sin$ and $\cos$ functions.

#### Calculating points
The new coordinates ($x_1$, $y_1$) are calculated using this code:
```c
x1 = (float)radiusX * cosf(theta) + 0.5;
y1 = (float)radiusY * sinf(theta) + 0.5;
```

The `+ 0.5` is a simple rounding technique to ensure that any number that is at the very least $0.5$ is rounded to $1$ (refer to how **C** casts between `float` and `int`).

#### Drawing Lines
The function uses `thickLineRGBA` (from **SDL2_gfx**) to draw lines between the calculated points. It draws lines from the previous point to the new point in all four quadrants.

- First  quadrant: $(x_0 + x, y_0 - y)$ to $(x_0 + x_1, y_0 − y_1)$
- Second quadrant: $(x_0 − x, y_0 − y)$ to $(x_0 − x_1, y_0 − y_1)$
- Third  quadrant: $(x_0 − x, y_0 + y)$ to $(x_0 − x_1, y_0 + y_1)$
- Fourth quadrant: $(x_0 + x, y_0 + y)$ to $(x_0 + x_1, y_0 + y_1)$

### Completing the arc
After the loop, there is a check to ensure that the arc is completed correctly. If the last calculated x-coordinate is not zero, it draws the final lines to ensure the ellipse is closed properly.

```c
// arc did not finish because of rounding, so finish the arc
if (x != 0) {
    x = 0;

    thickLineRGBA(r, x0 + x, y0 - y, x0 + x1, y0 - y1, ELLIPSIS_LINE_WIDTH, 0, 0, 0, 255);
    thickLineRGBA(r, x0 - x, y0 - y, x0 - x1, y0 - y1, ELLIPSIS_LINE_WIDTH, 0, 0, 0, 255);
    thickLineRGBA(r, x0 - x, y0 + y, x0 - x1, y0 + y1, ELLIPSIS_LINE_WIDTH, 0, 0, 0, 255);
    thickLineRGBA(r, x0 + x, y0 + y, x0 + x1, y0 + y1, ELLIPSIS_LINE_WIDTH, 0, 0, 0, 255);
}
```

