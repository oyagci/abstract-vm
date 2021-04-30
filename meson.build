project('abstract-vm', 'cpp', version: '0.0.1',
  default_options: [
	'cpp_std=c++17',
	'werror=true'
  ])

cmake = import('cmake')

fmt_var = cmake.subproject_options()
fmt_var.add_cmake_defines({ 'BUILD_SHARED_LIBS': true })
fmt_lib = cmake.subproject('fmt', options: fmt_var)
fmt_dep = fmt_lib.dependency('fmt')
fmt_inc = include_directories('subprojects/fmt/include')

subdir('abstract-vm')
subdir('runtime')
subdir('tests')
