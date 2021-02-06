#include "gtest/gtest.h"
#include "src/Interpreter.hpp"
#include "src/Parser.hpp"

using namespace avm;

TEST(Precision, Add_Float_Int)
{
	Interpreter l_interpreter;

	char const *l_input =
		"push int8(1)\n"
		"push float(42.42)\n"
		"add\n"
		"assert float(43.00)\n";

	Lexer l_lexer;
	l_lexer.Run(l_input);

	Parser l_parser(l_lexer.GetTokens());
	auto l_program = l_parser.Run();

	ASSERT_THROW({
		auto l_instruction = l_program->GetNextInstruction();
		while (l_instruction)
		{
			l_interpreter.Evaluate(*l_instruction);
			l_instruction = l_program->GetNextInstruction();
		}
	}, avm::AssertError);
}
