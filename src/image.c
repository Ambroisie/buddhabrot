#include "image.h"

#include <stdlib.h>

struct image *create_image(size_t w, size_t h) {
    // FIXME: not checking for overflow
    struct image *i = malloc(w * h * sizeof(*i->buf) + sizeof(*i));

    if (!i)
        return NULL;

    i->w = w;
    i->h = h;

    return i;
}

void destroy_image(struct image *image) {
    free(image);
}
