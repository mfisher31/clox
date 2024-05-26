project ('luis', [ 'c', 'cpp' ],
  version : '0.1.0',
  default_options : [ 
    'warning_level=3',
    'c_std=c11',
    'cpp_std=c++20'
  ])

exe = executable ('luis', 
  '''
    src/chunk.cpp
    src/compiler.cpp
    src/debug.cpp
    src/memory.cpp
    src/object.cpp
    src/scanner.cpp
    src/table.cpp
    src/value.cpp
    src/vm.cpp
    src/main.cpp
  '''.split(),
  install : true)

test('basic', exe)
