#ifndef IMAGE_H
#define IMAGE_H

#include <stddef.h>

struct image {
    size_t w;
    size_t h;
    struct pixel {
        unsigned char r;
        unsigned char g;
        unsigned char b;
    } buf[];
};

struct image *create_image(size_t w, size_t h);
void destroy_image(struct image *image);

static inline size_t to_index(size_t h, size_t w, const struct image *image) {
    return h * image->w + w;
}

#endif /* !IMAGE_H */
