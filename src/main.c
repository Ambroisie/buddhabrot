#include <err.h>
#include <stdio.h>
#include <stdlib.h>

#include "image.h"
#include "ppm.h"

static void fill_image(struct image *image) {
    for (size_t i = 0; i < image->h; ++i) {
        for (size_t j = 0; j < image->w; ++j) {
            struct pixel *p = &image->buf[i * image->w + j];
            p->r = 255 * i / image->h;
            p->g = 255 * j / image->w;
            p->g = 255 * (i + j) / (image->h + image->w);
        }
    }
}

int main(void) {
    struct image *image = create_image(960, 540);
    if (!image)
        err(EXIT_FAILURE, "could not allocate image");

    fill_image(image);
    print_ppm(image, stdout);
    destroy_image(image);

    return EXIT_SUCCESS;
}
