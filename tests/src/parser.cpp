#include "gtest/gtest.h"
#include "avm.hpp"
#include "src/Parser.hpp"

using namespace avm;

TEST(Sanity, Empty)
{
	Parser l_p = Parser(List<Token>());

	UniquePtr<ast::Program> l_program = l_p.Run();
	UniquePtr<const ast::Instruction> l_ins = l_program->GetNextInstruction();

	ASSERT_EQ(l_ins, nullptr);
}

TEST(Sanity, Simple_Push_Int8)
{
	Parser l_p({
		{ TokenType::PUSH,   "push", NullOpt, 1 },
		{ TokenType::INT8,   "int8", NullOpt, 1 },
		{ TokenType::LPAREN, "(",    NullOpt, 1 },
		{ TokenType::NUMBER, "42",   "42",    1 },
		{ TokenType::RPAREN, ")",    NullOpt, 1 },
	});

	UniquePtr<ast::Program> l_program = l_p.Run();
	UniquePtr<const ast::Instruction> l_ins = l_program->GetNextInstruction();

	ASSERT_NE(dynamic_cast<const ast::InstructionWithValue *>(l_ins.get()), nullptr);
	ASSERT_EQ(l_ins->GetType(), ast::Instruction::InstructionType::PUSH);
}
