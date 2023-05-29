testProgs = [
    'Mouse Ray Casting',
    'Timing',
    'Check Util',
    'Create Window',
    'Lua Repl',
    'Map Load',
    'Note Tools Test',
    'Layout Test',
]

if backend != 'headless'
    testProgs += ['UI Hook Trigger', 'Text Render']
endif


foreach p : testProgs
    canonical = p.replace(' ', '')
    srcp = files(canonical + '.cc')
    exe = executable(
        canonical,
        srcp,
        include_directories: incdir,
        dependencies: deps,
        override_options: ['c_std=c17', 'cpp_std=c++20'],
        link_with: atlib,
    )
    test(p, exe)
endforeach
