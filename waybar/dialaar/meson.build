project(
    'Dialaar', 'cpp',
    version: '0.1.0',
    license: 'MIT',
)

executable('bin', [
        'src/main.cpp',
        'src/wifi/gui.cpp',
        'src/wifi/entry.cpp',
        'src/wifi/config.cpp',
        'src/elements.cpp'
    ],
    dependencies: [
        dependency('gtkmm-4.0'),
        dependency('libnl-3.0'),
        dependency('libnl-genl-3.0')
    ],
)
