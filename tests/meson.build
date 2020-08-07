# Tests

googletest = cmake.subproject('googletest')

tests_srcs = [ 'src/main.cpp' ]
tests_incs = [
  include_directories('.'),
  include_directories('src'),
  include_directories('../abstract-vm'),
  include_directories('../subprojects/googletest/googletest/include'),
]
test_deps = [
  googletest.dependency('gtest'),
]

exe = executable('test-abstractvm',
  tests_srcs,
  include_directories: tests_incs,
  link_with: abstractvm_lib,
  dependencies: test_deps
)

test('all tests', exe)
