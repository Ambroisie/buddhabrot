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
    struct window {
        float min_x;
        float max_x;
        float min_y;
        float max_y;
    } window;

    size_t max_iter;
    size_t repeats;

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

static bool compute_orbit(struct my_complex z,
        struct orbit *o, size_t max_iter) {
    float r = 0.0;
    float i = 0.0;

    for (o->len = 0; o->len < max_iter; ++o->len) {
        o->values[o->len].r = r;
        o->values[o->len].i = i;

        float xtemp = r * r - i * i + z.r;
        i = 2 * r * i + z.i;
        r = xtemp;

        if (r * r + i * i >= 4)
            return false; // Diverges
    }

    return true;
}

static float orbit_probability(const struct window *w, const struct orbit *o) {
    for (size_t i = 0; i < o->len; ++i) {
        if (w->min_x <= o->values[i].r && o->values[i].r <= w->max_x
            && w->min_y <= o->values[i].i && o->values[i].i <= w->max_y)
            return 1;
    }

    return FLT_MIN; // Cannot be 0 because of sampler
}

static float transition_probability(const struct orbit *src,
        const struct orbit *dst, size_t max_iter) {
    return (1 - (max_iter - src->len) / max_iter)
        / (1 - (max_iter - dst->len) / max_iter);
}

static bool mutate_near(const struct orbit *current,
        struct orbit *new, size_t max_iter) {
    struct my_complex next = current->values[1]; // Always the first one after 0

    const double magnifictation = 1; // FIXME: proportional to magnification

    const double r1 = (1.f / magnifictation) * 0.0001;
    const double r2 = (1.f / magnifictation) * 0.1;

    const double phi =  M_PI * 2 * random() / LONG_MAX;
    const double r = r2 * exp(-log(r2 / r1) * random() / LONG_MAX);

    next.r += r * cos(phi);
    next.i += r * sin(phi);

    return compute_orbit(next, new, max_iter);
}

static bool mutate_any(struct orbit *new, size_t max_iter) {
    struct my_complex next = {
        2 - 4. * random() / LONG_MAX,
        2 - 4. * random() / LONG_MAX,
    };

    while (next.r * next.r + next.i * next.i >= 4) {
        next.r = 2 - 4. * random() / LONG_MAX;
        next.i = 2 - 4. * random() / LONG_MAX;
    }

    return compute_orbit(next, new, max_iter);
}

static void mutate(const struct data *d, struct orbit *current) {
    struct orbit *new =
        calloc(1, d->max_iter * sizeof(*new->values) + sizeof(*new));
    if (!new)
        err(EXIT_FAILURE, "could not allocate orbit data");

    // Only consider values that diverge
    if (random() < LONG_MAX / 5)
        while (!mutate_any(new, d->max_iter))
            continue;
    else
        while (!mutate_near(current, new, d->max_iter))
            continue;

    const float prob_cur = orbit_probability(&d->window, current);
    const float prob_new = orbit_probability(&d->window, new);

    const float t_new_cur = transition_probability(new, current, d->max_iter);
    const float t_cur_new = transition_probability(current, new, d->max_iter);

    const float alpha = (prob_new * t_cur_new) / (prob_cur * t_new_cur);

    if (alpha > (float)random() / LONG_MAX)
        memcpy(current, new, d->max_iter * sizeof(*new->values) + sizeof(*new));

    free(new);
}

static size_t inv_scale(float min, float max, float x, size_t width) {
    return (x - min) / (max - min) * width;
}

static void compute_buddahbrot(struct data *d) {
    // FIXME: does not check overflow
    struct orbit *o = calloc(1, d->max_iter * sizeof(*o->values) + sizeof(*o));
    if (!o)
        err(EXIT_FAILURE, "could not allocate orbit data");

    while (true)
        if (mutate_any(o, d->max_iter))
            break; // Initialize with correct value

    for (size_t i = 0; i < d->repeats; ++i) {
        mutate(d, o); // Computes its orbit

        for (size_t i = 0; i < o->len; ++i) {
            size_t w = inv_scale(d->window.min_x, d->window.max_x,
                    o->values[i].r, d->w);
            size_t h = inv_scale(d->window.min_y, d->window.max_y,
                    o->values[i].i, d->h);
            d->buf[data_index(h, w, d)] += 1;
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

void buddhabrot(struct image *i, size_t max_iter, size_t repeats) {
    struct data *d = calloc(1, sizeof(*d->buf) * i->w * i->h + sizeof(*d));
    if (!d)
        err(EXIT_FAILURE, "could not allocate buddhabrot data");

    d->h = i->h;
    d->w = i->w;
    d->max_iter = max_iter;
    d->repeats = repeats;

    // FIXME" make it user-definable
    d->window.min_x = -2.5;
    d->window.max_x = 1.0;
    d->window.min_y = -1.0;
    d->window.max_y = 1.0;

    compute_buddahbrot(d);
    fillout_image(d, i);

    free(d);
}
