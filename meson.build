project(
    'Almost There Core Game Components',
    'cpp',
    'c',
    version: '1.0.0',
    license: 'GPL-3.0-or-later',
)

src = []
backend = get_option('backlib')
# Backends are not linked in this repository which only produces static library
# Link them in the production branch
incdir = include_directories(['inc', 'src'])
subdir('src')
subdir('libs')
deps = []

if backend == 'glfw'
    deps += dependency('glfw3')
    deps += dependency('freetype')
endif
deps += dependency('spdlog')
foreach name : ['lua', 'lua5.4', 'lua-5.4', 'lua54']
    luaDep = dependency(name, required: false)
    if luaDep.found()
        deps += luaDep
        break
    endif
endforeach
if not luaDep.found()
    error('No Lua found. Almost There requires Lua to run.')
endif
atlib = static_library(
    'atgame',
    src,
    include_directories: incdir,
    dependencies: deps,
    override_options: ['c_std=c17', 'cpp_std=c++20'],
)
if backend != 'nothing'
    shared_library(
        'atgame',
        src,
        include_directories: incdir,
        dependencies: deps,
        override_options: ['c_std=c17', 'cpp_std=c++20'],
    )
    subdir('test')
else
    message(
        'Shared libraries and tests are not available when backend library specified as \'nothing\'.',
    )
endif

atgame = executable(
    'at',
    files('src/boot/Main.cc'),
    dependencies: deps,
    include_directories: incdir,
    link_with: atlib,
)
