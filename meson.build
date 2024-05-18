project ('clox', [ 'c', 'cpp' ],
  version : '0.1',
  default_options : [ 'warning_level=3' ])

exe = executable ('clox', 'src/main.cpp',
  install : true)

test('basic', exe)
