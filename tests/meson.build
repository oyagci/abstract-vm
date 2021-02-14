# Tests

googletest = cmake.subproject('googletest')

tests_srcs = [
  'src/main.cpp',
  'src/lexer.cpp',
]
tests_incs = [
  include_directories('.'),
  include_directories('src'),
  include_directories('../abstract-vm'),
  include_directories('../subprojects/googletest/googletest/include'),
]
test_deps = [
  googletest.dependency('gtest'),
  fmt_dep
]

exe = executable('test-abstractvm',
  tests_srcs,
  include_directories: tests_incs,
  link_with: abstractvm_lib,
  dependencies: test_deps
)

test('all tests', exe)

operands_src = [ 'src/main.cpp', 'src/operands.cpp' ]
operands = executable('test-operands',
  operands_src,
  include_directories: tests_incs,
  link_with: abstractvm_lib,
  dependencies: test_deps)


test('operands', operands)

parser_src = [ 'src/main.cpp', 'src/parser.cpp' ]
parser = executable('test-parser',
  parser_src,
  include_directories: tests_incs,
  link_with: abstractvm_lib,
  dependencies: test_deps)


test('parser', parser)


precision_src = [ 'src/main.cpp', 'src/precision.cpp' ]
precision = executable('test-precision',
  precision_src,
  include_directories: tests_incs,
  link_with: abstractvm_lib,
  dependencies: test_deps)


test('precision', precision)

programs_src = [ 'src/main.cpp', 'src/programs.cpp' ]
programs = executable('test-programs',
  programs_src,
  include_directories: tests_incs,
  link_with: abstractvm_lib,
  dependencies: test_deps)


test('programs', programs)
