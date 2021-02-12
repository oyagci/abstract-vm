#include <unistd.h>
#include <iostream>
#include "avm.hpp"
#include "src/Lexer.hpp"
#include "src/Parser.hpp"
#include "src/Interpreter.hpp"

int readline(std::string &p_result)
{
	std::getline(std::cin, p_result);

	if (std::cin.eof()) return 0;
	if (std::cin.fail()) return -1;

	return 1;
}

int RunRepl()
{
	avm::Lexer l_lexer;
	avm::Interpreter l_interpreter;

	while (true)
	{
		// Print prompt
		fmt::print("> ");
		std::cout << std::flush;

		// Read input
		std::string l_line;
		int res = readline(l_line);

		if (res <= 0 || l_line == ";;")
			break;

		l_lexer.Run(l_line + "\n");

		avm::Parser l_parser(l_lexer.GetTokens());
		avm::UniquePtr<avm::ast::Program> l_program = l_parser.Run();
		avm::UniquePtr<const avm::ast::Instruction> l_instruction = l_program->GetNextInstruction();
		while (l_instruction)
		{
			try
			{
				l_interpreter.Evaluate(*l_instruction);
			}
			catch (std::exception const &e)
			{
				fmt::print("Error: {}\n", e.what());
			}
			l_instruction = l_program->GetNextInstruction();
		}
	}
	return 0;
}

int RunFromFile(char const *const path)
{
	avm::Lexer l_lexer;

	l_lexer.RunFile(path);

	if (!avm::Lexer::HadError())
	{
		avm::Parser l_parser(l_lexer.GetTokens());

		auto l_program = l_parser.Run();

		avm::Interpreter l_interpreter;

		auto l_instruction = l_program->GetNextInstruction();
		while (l_instruction)
		{
			try
			{
				l_interpreter.Evaluate(*l_instruction);
			}
			catch (std::exception const &e)
			{
				fmt::print("Fatal Error: {}\n", e.what());
				break ;
			}
			l_instruction = l_program->GetNextInstruction();
		}

		return 0;
	}

	return 1;
}

int main(int ac, char *av[])
{
	if (ac <= 1)
	{
		return RunRepl();
	}
	else
	{
		return RunFromFile(av[1]);
	}
}
