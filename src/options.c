#include "options.h"

#include <err.h>
#include <stdlib.h>
#include <unistd.h>

struct options parse_options(int *argc, char **argv[]) {
    struct options opts = {
        .output = stdout,
        .w = 960,
        .h = 540,
        .max_iter = 100,
    };

    opterr = 0; // Deactivate error message from `getopt`

    int opt;
    while ((opt = getopt(*argc, *argv, "h:m:o:w:")) != -1) {
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
        case 'w':
            if (!sscanf(optarg, "%zu", &opts.w))
                errx(EXIT_FAILURE, "could not parse width");
            break;
        default:
            fprintf(stderr,
                    "Usage: %s [-o FILE] [-m MAX_ITER] [-h HEIGHT] [-w WIDTH]\n",
                    (*argv)[0]);
            exit(EXIT_FAILURE);
        }
    }


    *argc -= optind;
    *argv += optind;

    return opts;
}
