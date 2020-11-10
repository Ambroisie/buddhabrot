#include <err.h>
#include <stdio.h>
#include <stdlib.h>

#include "image.h"
#include "mandelbrot.h"
#include "options.h"
#include "ppm.h"

int main(int argc, char *argv[]) {
    struct options opt = parse_options(&argc, &argv);

    struct image *image = create_image(opt.w, opt.h);
    if (!image)
        err(EXIT_FAILURE, "could not allocate image");

    mandelbrot(image, opt.max_iter);
    print_ppm(image, opt.output);

    fclose(opt.output);
    destroy_image(image);

    return EXIT_SUCCESS;
}
