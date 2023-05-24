testProgs = [
'Mouse Ray Casting',
'Score Calc',
'Timing',
'Check Util',
'Create Window',
'Lua Repl',
'Map Load',
]

if backend != 'headless'
  testProgs += ['UI Hook Trigger', 'Map Play']
endif


foreach p : testProgs
  canonical = p.replace(' ', '')
  srcp = src + files(canonical + '.cc')
  exe = executable(canonical, srcp, include_directories : incdir, dependencies: deps, override_options : ['c_std=c17', 'cpp_std=c++20'])
  test(p, exe)
endforeach