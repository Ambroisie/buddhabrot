project(
  'buddhabrot',
  'c',
  version : '0.1',
  default_options : [
    'warning_level=3',
    'c_std=c99',
  ],
)

sources = [
  'src/buddhabrot.c',
  'src/image.c',
  'src/main.c',
  'src/mandelbrot.c',
  'src/options.c',
  'src/ppm.c',
]

executable(
  'buddhabrot',
  sources : sources,
  c_args : '-D_XOPEN_SOURCE',
)
