project ('clox', [ 'c', 'cpp' ],
  version : '0.1',
  default_options : [ 
    'warning_level=3',
    'c_std=c99' 
  ])

exe = executable ('clox', 
  '''
    src/chunk.c
    src/compiler.c
    src/debug.c
    src/memory.c
    src/object.c
    src/scanner.c
    src/table.c
    src/value.c
    src/vm.c
    src/main.cpp
  '''.split(),
  install : true)

test('basic', exe)
