project(
    'waybar_modules', 'c',
    version: '0.1.0',
    license: 'MIT',
)

shared_library('audio',
    ['Audio/main.c'],
    dependencies: [
        dependency('gtk+-3.0', version : ['>=3.22.0'])
    ],
    name_prefix: ''
)

shared_library('theme-switcher',
    ['ThemeSwitcher/main.c'],
    dependencies: [
        dependency('gtk+-3.0', version : ['>=3.22.0'])
    ],
    name_prefix: ''
)
