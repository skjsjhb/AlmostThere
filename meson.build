project('Almost There Core Game Components', 'cpp', version : '1.0.0', license : 'GPL-3.0-or-later')
src = []
incdir = include_directories(['inc', 'src'])
subdir('src')
static_library('atgame', src, include_directories : incdir)
shared_library('atgame', src, include_directories : incdir)
subdir('test')