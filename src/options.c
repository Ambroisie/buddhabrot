#include "options.h"

#include <err.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ARR_SIZE(Arr) (sizeof(Arr) / sizeof(*Arr))

static const char *methods[] = {
    [MANDELBROT] = "mandelbrot",
    [BUDDHABROT] = "buddhabrot",
};

struct options parse_options(int *argc, char **argv[]) {
    struct options opts = {
        .output = stdout,
        .w = 960,
        .h = 540,
        .max_iter = 100,
        .render = BUDDHABROT,
    };

    opterr = 0; // Deactivate error message from `getopt`

    int opt;
    while ((opt = getopt(*argc, *argv, "h:m:o:r:w:")) != -1) {
        switch (opt) {
        case 'h':
            if (!sscanf(optarg, "%zu", &opts.h))
                errx(EXIT_FAILURE, "could not parse height");
            break;
        case 'm':
            if (!sscanf(optarg, "%zu", &opts.max_iter))
                errx(EXIT_FAILURE, "could not parse max_iter");
            break;
        case 'o':
            if (!freopen(optarg, "w", opts.output))
                err(EXIT_FAILURE, "could not open output file");
            break;
        case 'r': {
            size_t i;
            for (i = 0; i < ARR_SIZE(methods); ++i)
                if (!strcmp(methods[i], optarg)) {
                    opts.render = i;
                    break;
                }
            if (i == ARR_SIZE(methods))
                errx(EXIT_FAILURE, "could not parse render");
            break;
        }
        case 'w':
            if (!sscanf(optarg, "%zu", &opts.w))
                errx(EXIT_FAILURE, "could not parse width");
            break;
        default:
            fprintf(stderr,
                    "Usage: %s "
                    "[-o FILE] [-i MAX_ITER] [-h HEIGHT] [-w WIDTH] "
                    "[-r <buddhabrot|mandelbrot>]\n",
                    (*argv)[0]);
            exit(EXIT_FAILURE);
        }
    }


    *argc -= optind;
    *argv += optind;

    return opts;
}
