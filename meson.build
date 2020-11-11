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

cc = meson.get_compiler('c')
deps = [
  cc.find_library('m', required : false),
]

executable(
  'buddhabrot',
  sources : sources,
  c_args : '-D_XOPEN_SOURCE=500',
  dependencies : deps,
)
