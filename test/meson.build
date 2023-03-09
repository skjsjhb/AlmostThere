progs = ['Mouse Ray Casting', 'Score Calc', 'Timing', 'Check Util', 'Animation', 'Schedule']

foreach p : progs
  canonical = p.replace(' ', '')
  srcp = src + files(canonical + '.cc')
  exe = executable(canonical, srcp, include_directories : incdir)
  test(p, exe)
endforeach