#include "ppm.h"

#include <stdio.h>
#include "image.h"

void print_ppm(const struct image *image, FILE *f) {
    // Header
    fputs("P3\n", f);
    fprintf(f, "%zu %zu\n", image->w, image->h);
    fputs("255\n", f);

    // One row per line
    for (size_t i = 0; i < image->h; ++i) {
        for (size_t j = 0; j < image->w; ++j) {
            if (j != 0)
                fputc(' ', f);
            const struct pixel *p = &image->buf[to_index(i, j, image)];
            fprintf(f, "%hhu %hhu %hhu", p->r, p->g, p->b);
        }
        fputc('\n', f);
    }
}
