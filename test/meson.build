testProgs = [
'Mouse Ray Casting',
'Score Calc',
'Timing',
'Check Util',
'Animation',
'Schedule',
'Create Window',
'Note Pos',
'Note Draw'
]


foreach p : testProgs
  canonical = p.replace(' ', '')
  srcp = src + files(canonical + '.cc')
  exe = executable(canonical, srcp, include_directories : incdir, dependencies: deps, override_options : ['c_std=c17', 'cpp_std=c++20'])
  test(p, exe)
endforeach