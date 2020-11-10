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

#endif /* !IMAGE_H */
