project('libpsu', 'c',
    version: '0.1',
    license: 'GPL2',
    default_options: [
        'warning_level=3',
        'werror=true',
    ],
    meson_version: '>=0.40'
)

sources = [
    'psu.c',
]

cc = meson.get_compiler('c')
deps = [
    cc.find_library('misc-utils'),
    dependency('libobmc-i2c'),
    dependency('liblog'),
]

install_headers(
    'psu.h',
    subdir: 'openbmc',
)

psu_lib = shared_library('psu',
    sources,
    dependencies: deps,
    version: meson.project_version(),
    install: true,
)

pkg = import('pkgconfig')
pkg.generate(
    libraries: [ psu_lib ],
    name: meson.project_name(),
    version: meson.project_version(),
    description: 'PSU Device Access Library'
)
