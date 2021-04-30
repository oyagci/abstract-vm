CXX         = clang++
CXXFLAGS    = -std=c++17 -Wall -Wextra -Werror

SRCDIR = src
OBJDIR = obj

SOURCES_RAW = format.cc
OBJECTS_RAW = $(SOURCES_RAW:.cc=.o)
DEPS_RAW =

OBJECTS = $(addprefix $(OBJDIR)/,$(OBJECTS_RAW))
DEPS = $(addprefix $(OBJDIR)/,$(DEPS_RAW))
INCLUDES = -I./include

NAME = libfmt.so

# ============================================================================ #

.PHONY: all clean fclean re fmt
all:$(NAME)

$(NAME): $(OBJECTS) $(DEPS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(NAME) -shared

$(OBJDIR)/%.o: $(SRCDIR)/%.cc
	@mkdir -p $(OBJDIR)
	$(CXX) -fPIC -c $< -o $@ $(CXXFLAGS) $(INCLUDES)

clean:
	$(RM) $(OBJECTS)
	$(RM) -R $(OBJDIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all
