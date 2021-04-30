CXX         = clang++
CXXFLAGS    = -std=c++17 -Wall -Wextra -Werror

OBJDIR		= obj
SRCDIR		= src
DEPSDIR		= include

SOURCES_RAW	=          \
	Interpreter.cpp    \
	Lexer.cpp          \
	Operand.cpp        \
	OperandFactory.cpp \
	Parser.cpp         \
	abstractvm.cpp     \
    ast/Instruction.cpp\
    ast/Value.cpp
OBJECTS_RAW	= $(SOURCES_RAW:.cpp=.o)
DEPS_RAW	=          \
	IOperand.hpp       \
	Interpreter.hpp    \
	Lexer.hpp          \
	Operand.hpp        \
	OperandFactory.hpp \
	Parser.hpp         \
	abstractvm.hpp     \
	ast/Instruction.hpp\
	ast/Value.hpp

OBJECTS		= $(addprefix $(OBJDIR)/,$(OBJECTS_RAW))
DEPS		= $(addprefix ./src/,$(DEPS_RAW))

INCLUDES	= -I./include/ -I./src -I../subprojects/fmt/include

NAME		= libavm.so

.PHONY: all clean fclean re fmt
all: $(NAME)

$(NAME): $(OBJECTS) $(DEPS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(NAME) -shared

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)/ast
	$(CXX) -fPIC -c $< -o $@ $(CXXFLAGS) $(INCLUDES)

clean:
	$(RM) $(OBJECTS)
	$(RM) -R $(OBJDIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all
