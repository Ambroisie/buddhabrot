#include "buddhabrot.h"

#include <err.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

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

struct orbit {
    size_t len;
    struct my_complex {
        float r;
        float i;
    } values[];
};

static size_t data_index(size_t h, size_t w, const struct data *d) {
    return h * d->w + w;
}

static float scale(float min, float max, size_t x, size_t width) {
    return min + x * (max - min) / (width - 1);
}

static size_t inv_scale(float min, float max, float x, size_t width) {
    return (x - min) / (max - min) * (width - 1);
}

static bool compute_orbit(struct my_complex z,
        struct orbit *o, size_t max_iter) {
    float r = 0.0;
    float i = 0.0;

    for (o->len = 0; o->len < max_iter; ++o->len) {
        float xtemp = r * r - i * i + z.r;
        i = 2 * r * i + z.i;
        r = xtemp;

        if (r * r + i * i >= 4)
            return true; // Diverges

        o->values[o->len].r = r;
        o->values[o->len].i = i;
    }

    return false;
}

static void compute_buddahbrot(struct data *d) {
    // FIXME: does not check overflow
    struct orbit *o = calloc(1, d->max_iter * sizeof(*o->values) + sizeof(*o));
    if (!o)
        err(EXIT_FAILURE, "could not allocate orbit data");

    for (size_t i = 0; i < d->h; ++i) {
        for (size_t j = 0; j < d->w; ++j) {
            struct my_complex z = {
                scale(d->min_x, d->max_x, j, d->w),
                scale(d->min_y, d->max_y, i, d->h),
            };
            // Only keep if it diverges
            if (!compute_orbit(z, o, d->max_iter))
                continue;

            for (size_t i = 0; i < o->len; ++i) {
                size_t w = inv_scale(d->min_x, d->max_x,
                        o->values[i].r, d->w);
                size_t h = inv_scale(d->min_y, d->max_y,
                        o->values[i].i, d->h);
                if (w >= d->w || h >= d->h)
                    continue; // We've diverged, keep going just in case
                d->buf[data_index(h, w, d)] += 1;
            }
        }
    }

    free(o);
}

static void fillout_image(const struct data *d, struct image *i) {
    size_t max = 0;
    for (size_t ind = 0; ind < i->h * i->w; ++ind)
        if (max < d->buf[ind])
            max = d->buf[ind];

    for (size_t ind = 0; ind < i->h * i->w; ++ind) {
        unsigned char v = d->buf[ind] * 255 / max;
        i->buf[ind] = (struct pixel){ v, v, v };
    }
}

void buddhabrot(struct image *i, size_t max_iter) {
    struct data *d = calloc(1, sizeof(*d->buf) * i->w * i->h + sizeof(*d));
    if (!d)
        err(EXIT_FAILURE, "could not allocate buddhabrot data");

    d->h = i->h;
    d->w = i->w;
    d->max_iter = max_iter;

    // FIXME" make it user-definable
    d->min_x = -2.5;
    d->max_x = 1.0;
    d->min_y = -1.0;
    d->max_y = 1.0;

    compute_buddahbrot(d);
    fillout_image(d, i);

    free(d);
}
