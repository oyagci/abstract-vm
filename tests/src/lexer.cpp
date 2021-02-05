#include "gtest/gtest.h"
#include "avm.hpp"
#include "src/Lexer.hpp"

using namespace avm;

class LexerTest : public ::testing::Test
{
};

TEST_F(LexerTest, Program1)
{
	char const *l_source =
		"push int8(42)"
		"push int16(420)"
		"push int32(0)";

	Scanner l_scanner(l_source);
	List<Token> l_tokens = l_scanner.ScanTokens();

	List<TokenType> l_expectedTokens = {
		PUSH,  INT8, LPAREN, NUMBER, RPAREN,
		PUSH, INT16, LPAREN, NUMBER, RPAREN,
		PUSH, INT32, LPAREN, NUMBER, RPAREN,
	};

	for (size_t i = 0; i < l_tokens.size(); i++)
	{
		TokenType l_expected = l_expectedTokens.front();
		TokenType l_actual = l_tokens.front().m_type;

		ASSERT_EQ(l_expected, l_actual);

		l_expectedTokens.pop_front();
		l_tokens.pop_front();
	}
}

TEST_F(LexerTest, AllInstructions)
{
	char const *l_source =
		"push int8(42)\n"
		"push int16(420)\n"
		"push int32(0)\n"
		"pop\n"
		"add\n"
		"sub\n"
		"div\n"
		"mod\n"
		"print\n"
		"exit\n"
		"assert 20\n"
		";;\n"
		"; this is a comment";

	Scanner l_scanner(l_source);
	List<Token> l_tokens = l_scanner.ScanTokens();

	List<TokenType> l_expectedTokens = {
		PUSH,  INT8, LPAREN, NUMBER, RPAREN, NEWLINE,
		PUSH, INT16, LPAREN, NUMBER, RPAREN, NEWLINE,
		PUSH, INT32, LPAREN, NUMBER, RPAREN, NEWLINE,
		POP, NEWLINE,
		ADD, NEWLINE,
		SUB, NEWLINE,
		DIV, NEWLINE,
		MOD, NEWLINE,
		PRINT, NEWLINE,
		EXIT, NEWLINE,
		ASSERT, NUMBER, NEWLINE,
		INPUT_STOP, NEWLINE,
	};

	for (size_t i = 0; i < std::max(l_expectedTokens.size(), l_tokens.size()); i++)
	{
		TokenType l_expected = l_expectedTokens.front();
		TokenType l_actual = l_tokens.front().m_type;

		ASSERT_EQ(l_expected, l_actual);

		l_expectedTokens.pop_front();
		l_tokens.pop_front();
	}
}

TEST_F(LexerTest, Number)
{
	char const *l_source =
		"42\n"
		"1337\n"
		"0\n"
		"0.0\n"
		"-1\n"
		"-42\n"
		"-0";

	Scanner l_scanner(l_source);
	List<Token> l_tokens = l_scanner.ScanTokens();

	// 7 Numbers, 6 Newlines, 1 InputStop
	ASSERT_EQ(l_tokens.size(), 7U + 6U + 1U);

	for (size_t i = 0; i < l_tokens.size(); i++)
	{
		TokenType l_actual = l_tokens.front().m_type;

		ASSERT_EQ(NUMBER, l_actual);

		l_tokens.pop_front();
		l_tokens.pop_front();
	}
}
