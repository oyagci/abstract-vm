# Runtime Executable

runtime_srcs = [ 'src/main.cpp' ]
runtime_incs = [
  include_directories('.'),
  include_directories('src'),
  include_directories('../abstract-vm')
]
runtime_deps = [
  fmt_dep
]

executable('avm', runtime_srcs, include_directories: runtime_incs, link_with: abstractvm_lib, dependencies: fmt_dep)
