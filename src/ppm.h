#ifndef PPM_H
#define PPM_H

#include <stdio.h>

struct image; // Forward declaration

void print_ppm(const struct image *image, FILE *f);

#endif /* !PPM_H */
