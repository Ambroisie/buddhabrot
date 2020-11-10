#include <err.h>
#include <stdio.h>
#include <stdlib.h>

#include "image.h"
#include "options.h"
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

int main(int argc, char *argv[]) {
    struct options opt = parse_options(&argc, &argv);

    struct image *image = create_image(opt.w, opt.h);
    if (!image)
        err(EXIT_FAILURE, "could not allocate image");

    fill_image(image);
    print_ppm(image, opt.output);

    fclose(opt.output);
    destroy_image(image);

    return EXIT_SUCCESS;
}
