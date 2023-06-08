project('libxcvr', 'cpp',
    version: '0.1',
    license: 'GPL2',
    default_options: [
        'werror=true',
        'warning_level=3',
        'cpp_std=c++20',
    ],
    meson_version: '>=0.57.0'
)

cc = meson.get_compiler('cpp')
deps = [
    cc.find_library('misc-utils'),
]

headers = [
    'xcvr-optoe.hpp',
]

sources = [
    'xcvr-optoe.cpp',
]

install_headers(
    headers,
    subdir: 'openbmc',
)

xcvr_lib = shared_library('xcvr',
    sources,
    dependencies: deps,
    version: meson.project_version(),
    install: true,
)

pkg = import('pkgconfig')
pkg.generate(
    libraries: [ xcvr_lib ],
    name: meson.project_name(),
    version: meson.project_version(),
    description: 'XCVR Device Access Library'
)
