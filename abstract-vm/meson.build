# Abstract VM Library

abstract_incs = [
  include_directories('.'),
  include_directories('src'),
  include_directories('include'),
  fmt_inc,
]
abstract_srcs = [
  'src/abstractvm.cpp',
  'src/Lexer.cpp',
  'src/OperandFactory.cpp',
  'src/Interpreter.cpp',
  'src/Operand.cpp',
  'src/Parser.cpp',
  'src/ast/Instruction.cpp',
  'src/ast/Value.cpp',
]
abstract_deps = [
  fmt_dep
]

abstractvm_lib = library('abstractvm', abstract_srcs, include_directories: abstract_incs, dependencies: abstract_deps)
