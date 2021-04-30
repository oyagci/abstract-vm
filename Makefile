CXX         = clang++
CXXFLAGS    = -std=c++17 -Wall -Wextra -Werror

SRCDIR = runtime/src
OBJDIR = runtime/obj

SOURCES_RAW = main.cpp
OBJECTS_RAW = $(SOURCES_RAW:.cpp=.o)
DEPS_RAW =

OBJECTS = $(addprefix $(OBJDIR)/,$(OBJECTS_RAW))
DEPS = $(addprefix $(OBJDIR)/,$(DEPS_RAW))
INCLUDES = -I./abstract-vm/ -I./subprojects/fmt/include

NAME = avm

# ============================================================================ #

.PHONY: all clean fclean re fmt
all: fmt libavm.a $(NAME)

fmt:
	make -C ./subprojects/fmt/

$(NAME): $(OBJECTS) $(DEPS)
	$(CXX) subprojects/fmt/libfmt.so abstract-vm/libavm.so $(CXXFLAGS) $(OBJECTS) -o $(NAME)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) -c $< -o $@ $(CXXFLAGS) $(INCLUDES)

clean:
	$(RM) $(OBJECTS)
	$(RM) -R $(OBJDIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

libavm.a:
	make -C ./abstract-vm
