#include <gtest/gtest.h>
#include "avm.hpp"
#include "src/Lexer.hpp"
#include "src/Parser.hpp"
#include "src/Interpreter.hpp"

int RunFromSrc(char const *const src)
{
	avm::Lexer l_lexer;

	l_lexer.Run(src);

	if (!l_lexer.HadError())
	{
		avm::Parser l_parser(l_lexer, l_lexer.GetTokens());

		auto l_program = l_parser.Run();

		avm::Interpreter l_interpreter;

		auto l_instruction = l_program->GetNextInstruction();
		while (l_instruction)
		{
			l_interpreter.Evaluate(*l_instruction);
			l_instruction = l_program->GetNextInstruction();
		}

		return 0;
	}
	return 1;
}

TEST(Program, Test1)
{
	char const *const l_source =
		"push int32(42)\n"
		"push int32(33)\n"
		"add ;poney\n"
		"push float(44.55)\n"
		"mul\n"
		"push double(42.42)\n"
		";commentaire de ouf\n"
		"push int32(42)\n"
		"dump\n"
		"pop\n"
		"assert double(42.42)\n"
		"exit\n";

	ASSERT_EQ(RunFromSrc(l_source), 0);
}

TEST(Program, DivByZero)
{
	char const *const l_source =
		"push int32(0)\n"
		"push int32(0)\n"
		"div\n"
		"exit\n";

	ASSERT_THROW(RunFromSrc(l_source), avm::DivisionByZero);
}

TEST(Program, Overflow)
{
	char const *const l_source =
		"push int16(9999999999999999999999999999999999999999)\n"
		"exit\n";

	ASSERT_THROW(RunFromSrc(l_source), std::runtime_error);
}

TEST(Program, SyntaxErr)
{
	char const *const l_source =
		"push int16(32 ;)\n"
		"pu int(32))\n"
		"exit\n";

	ASSERT_EQ(RunFromSrc(l_source), 1);
}

TEST(Program, EmptyStack)
{
	char const *const l_source =
		"pop\n"
		"exit\n";

	ASSERT_THROW(RunFromSrc(l_source), avm::EmptyStackError);
}

TEST(Program, Assert)
{
	char const *const l_source =
		"push int32(42)\n"
		"assert int32(0)\n"
		"exit\n";

	ASSERT_THROW(RunFromSrc(l_source), avm::AssertError);
}

TEST(Program, AddStackTooSmall)
{
	char const *const l_source =
		"push int32(42)\n"
		"add\n"
		"exit\n";

	ASSERT_THROW(RunFromSrc(l_source), avm::EmptyStackError);
}

TEST(Program, Plop)
{
	char const *const l_source =
		"push int8(33)\n"
		"push int8(112)\n"
		"push int8(111)\n"
		"push int8(108)\n"
		"push int8(112)\n"
		"print\n"
		"pop\n"
		"print\n"
		"pop\n"
		"print\n"
		"pop\n"
		"print\n"
		"pop\n"
		"print\n"
		"pop\n"
		"exit;\n";

	ASSERT_EQ(RunFromSrc(l_source), 0);
}
