#ifndef OPTIONS_H
#define OPTIONS_H

#include <stddef.h>
#include <stdio.h>

struct options {
    FILE *output;
    size_t w;
    size_t h;
};

struct options parse_options(int *argc, char **argv[]);

#endif /* !OPTIONS_H */
