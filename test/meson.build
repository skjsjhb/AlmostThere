testProgs = [
    'Mouse Ray Casting',
    'Timing',
    'Check Util',
    'Create Window',
    'Lua Repl',
    'Map Load',
    'Note Tools Test',
    'Layout Test',
    'Event System',
]

if backend != 'headless'
    testProgs += ['Text Render', 'HUD Render', 'Audio Test']
endif


foreach p : testProgs
    canonical = p.replace(' ', '')
    srcp = files(canonical + '.cc')
    exe = executable(
        canonical,
        srcp,
        include_directories: incdir,
        dependencies: deps,
        override_options: optn,
        link_with: atlib,
        link_args: linkarg,
    )
    test(p, exe)
endforeach
