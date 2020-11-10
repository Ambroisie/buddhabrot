#include "mandelbrot.h"

#include <err.h>
#include <stddef.h>
#include <stdlib.h>

#include "image.h"

struct data {
    float min_x;
    float max_x;
    float min_y;
    float max_y;

    size_t max_iter;

    size_t h;
    size_t w;

    size_t buf[];
};

static size_t data_index(size_t h, size_t w, const struct data *d) {
    return h * d->w + w;
}

static void normalize_histogram(float *h, size_t max_iter) {
    float sum = 0;

    // Don't add divergent points to the total
    for (size_t i = 0; i < max_iter; ++i)
        sum += h[i];

    float running = 0;
    for (size_t i = 0; i <= max_iter; ++i) {
        running += h[i];
        h[i] = running / sum;
    }
}

static void compute_histogram(const struct data *d, float *h) {
    // Histogram must be zero-initialized
    for (size_t i = 0; i < d->h; ++i) {
        for (size_t j = 0; j < d->w; ++j) {
            size_t ind = data_index(i, j, d);
            h[d->buf[ind]] += 1;
        }
    }

    normalize_histogram(h, d->max_iter);
}

static float scale(float min, float max, size_t x, size_t width) {
    return min + x * (max - min) / width;
}

static void compute_cycle(struct data *d, size_t h, size_t w) {
    float x = scale(d->min_x, d->max_x, w, d->w);
    float y = scale(d->min_y, d->max_y, h, d->h);


    float i = 0.0;
    float j = 0.0;
    size_t k = 0;

    while (i * i + j * j < 2 * 2 && k < d->max_iter) {
        float xtemp = i * i - j * j + x;
        j = 2 * i * j + y;
        i = xtemp;
        ++k;
    }

    d->buf[data_index(h, w, d)] =  k;
}

static void compute_mandelbrot(struct data *d) {
    for (size_t i = 0; i < d->h; ++i) {
        for (size_t j = 0; j < d->w; ++j) {
            compute_cycle(d, i, j);
        }
    }
}

static void apply_lut(struct pixel *p, float x) {
    const float x0 = 1.f / 4.f;
    const float x1 = 2.f / 4.f;
    const float x2 = 3.f / 4.f;

    if (x < x0)
        *p = (struct pixel){0, x / x0 * 255, 255};
    else if (x < x1)
        *p = (struct pixel){0, 255, (x1 - x) / x0 * 255};
    else if (x < x2)
        *p = (struct pixel){(x - x1) / x0 * 255, 255, 0};
    else if (x < 1)
        *p = (struct pixel){255, (1.f - x) / x0 * 255, 0};
    else
        *p = (struct pixel){0, 0, 0};

}

void mandelbrot(struct image *i, size_t max_iter) {
    struct data *d = malloc(sizeof(*d->buf) * i->w * i->h + sizeof(*d));
    if (!d)
        err(EXIT_FAILURE, "could not allocate mandelbrot data");
    float *h = calloc(sizeof(float), max_iter + 1);
    if (!h) {
        free(d);
        err(EXIT_FAILURE, "could not allocate histogram");
    }

    d->h = i->h;
    d->w = i->w;
    d->max_iter = max_iter;

    // FIXME" make it user-definable
    d->min_x = -2.5;
    d->max_x = 1.0;
    d->min_y = -1.0;
    d->max_y = 1.0;

    compute_mandelbrot(d);
    compute_histogram(d, h);

    for (size_t ind = 0; ind < i->h * i->w; ++ind)
        apply_lut(&i->buf[ind], h[d->buf[ind]]);

    free(d);
    free(h);
}
