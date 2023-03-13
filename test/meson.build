testProgs = [
'Mouse Ray Casting',
'Score Calc',
'Timing',
'Check Util',
'Animation',
'Schedule',
'Create Window'
]


foreach p : testProgs
  canonical = p.replace(' ', '')
  srcp = src + files(canonical + '.cc')
  exe = executable(canonical, srcp, include_directories : incdir, dependencies: deps)
  test(p, exe)
endforeach