#include "avm.hpp"
#include "src/Lexer.hpp"
#include "src/Parser.hpp"
#include "src/Interpreter.hpp"

int main(int ac, char *av[])
{
	avm::Lexer l_lexer;

	l_lexer.RunFile(av[1]);

	if (!avm::Lexer::HadError())
	{
		avm::Parser l_parser(l_lexer.GetTokens());

		auto l_program = l_parser.Run();

		avm::Interpreter l_interpreter;

		auto l_instruction = l_program->GetNextInstruction();
		while (l_instruction)
		{
			l_interpreter.Evaluate(*l_instruction);
			l_instruction = l_program->GetNextInstruction();
		}
	}

	return 0;
}
