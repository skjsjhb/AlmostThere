project('Almost There Core Game Components', 'cpp', 'c', version : '1.0.0', license : 'GPL-3.0-or-later')

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
endif
static_library('atgame', src, include_directories : incdir, dependencies: deps, override_options : ['c_std=c17', 'cpp_std=c++20'])
if backend != 'nothing'
  shared_library('atgame', src, include_directories : incdir, dependencies: deps, override_options : ['c_std=c17', 'cpp_std=c++20'])
  subdir('test')
else
  message('Shared libraries and tests are not available when backend library specified as \'nothing\'.')
endif
