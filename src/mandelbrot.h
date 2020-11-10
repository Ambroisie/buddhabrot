#ifndef MANDELBROT_H
#define MANDELBROT_H

#include <stddef.h>

struct image; // Forward declaration

void mandelbrot(struct image *image, size_t max_iter);

#endif /* !MANDELBROT_H */
