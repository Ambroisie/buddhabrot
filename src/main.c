#include <err.h>
#include <stdio.h>
#include <stdlib.h>

#include "buddhabrot.h"
#include "image.h"
#include "buddhabrot.h"
#include "mandelbrot.h"
#include "options.h"
#include "ppm.h"

int main(int argc, char *argv[]) {
    struct options opt = parse_options(&argc, &argv);

    struct image *image = create_image(opt.w, opt.h);
    if (!image)
        err(EXIT_FAILURE, "could not allocate image");

    switch (opt.render) {
    case BUDDHABROT:
        buddhabrot(image, opt.max_iter, 10000); // FIXME: allow user option
        break;
    case MANDELBROT:
        mandelbrot(image, opt.max_iter);
        break;
    }

    print_ppm(image, opt.output);

    fclose(opt.output);
    destroy_image(image);

    return EXIT_SUCCESS;
}
