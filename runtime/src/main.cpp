#include "avm.hpp"
#include "src/Lexer.hpp"

int main(int ac, char *av[])
{
	avm::Lexer l_lexer;

	l_lexer.RunFile(av[1]);

	return 0;
}
