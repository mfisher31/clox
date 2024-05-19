project ('clox', [ 'c', 'cpp' ],
  version : '0.1',
  default_options : [ 'warning_level=3' ])

exe = executable ('clox', 
  '''
    src/chunk.c
    src/debug.c
    src/memory.c
    src/value.c
    src/vm.c
    src/main.cpp
  '''.split(),
  install : true)

test('basic', exe)
