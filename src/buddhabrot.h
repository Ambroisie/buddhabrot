#ifndef BUDDHABROT_H
#define BUDDHABROT_H

#include <stddef.h>

struct image; // Forward declaration

void buddhabrot(struct image *i, size_t max_iter);

#endif /* !BUDDHABROT_H */
