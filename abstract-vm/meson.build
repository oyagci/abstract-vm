# Abstract VM Library

abstract_incs = [
  include_directories('.'),
  include_directories('src'),
  fmt_inc,
]
abstract_srcs = [ 'src/abstractvm.cpp' ]
abstract_deps = [
  fmt_dep
]

abstractvm_lib = library('abstractvm', abstract_srcs, include_directories: abstract_incs, dependencies: abstract_deps)
