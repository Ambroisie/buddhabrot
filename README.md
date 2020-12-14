# Buddhabrot

This is a very simple binary to create images of the [Mandelbrot
set](https://en.wikipedia.org/wiki/Mandelbrot_set) and [Buddhabrot
set](https://en.wikipedia.org/wiki/Buddhabrot). The final goal is to use the
[Metropolis-Hastings sampling
algorithm](https://en.wikipedia.org/wiki/Metropolis%E2%80%93Hastings_algorithm)
and [Simulated Annealing](https://en.wikipedia.org/wiki/Simulated_annealing).

## Building the project

You need [Meson](https://mesonbuild.com/) to build the project, and I recommend
using [Ninja](https://ninja-build.org/) as a build system. Issue the following
commands:

```sh
42sh$ meson build
42sh$ ninja -C build
```

This compiles the `buddhabrot` binary at the root of the `build` directory.

## Using the binary

By default, the binary writes the output image to its standard output, in the
[PPM file format](http://netpbm.sourceforge.net/doc/ppm.html).

You can choose which visualisation you want by using the `-r` options with
either `buddhabrot` or `mandelbrot`. Use `-m` to choose the maximum number of
iterated samples.

You can specify the height and width of the image by using `-h` and `-w`.

Sample usage:

```sh
42sh$ ./buddhabrot -r buddhabrot -o out.ppm  -m 1000 -h 1080 -w 1920
```

## Creating images

The binary outputs a grayscale image of the buddhabrot for the given sampling
rate.

The project's logo has been created by outputting three different buddhabrot
sets for `100`, `1000`, and `10000` samples. Once done, use the following
command to create the image (using
[ImageMagick](https://imagemagick.org/index.php)).

```sh
42sh$ convert \
    10000.ppm -channel R \
    1000.ppm -channel G \
    100.ppm -channel B \
    -combine -channel RGB \
    -alpha off -colorspace sRGB \
    out.png
```
