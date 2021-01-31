#pragma once
#include "abstractvm.hpp"
#include <fmt/format.h>
#include <fstream>
#include <sstream>

#define _Q(x) #x
#define QUOTE(x) _Q(x)

#define CASE_TOKEN(x) case x: return QUOTE(x); break;

namespace avm {

	enum TokenType {
		NONE = 0,

		SEMICOLON, NEWLINE,

		LPAREN, RPAREN,

		NUMBER, FLOAT_NUMBER,

		INT8, INT16, INT32, FLOAT, DOUBLE,

		PUSH, POP, DUMP, ASSERT, ADD, SUB, MUL, DIV, MOD, PRINT, EXIT,
		INPUT_STOP,
	};

	struct Token {
		TokenType m_type;
		String m_lexeme;
		Optional<String> m_literal;
		int m_line;

		Token(TokenType p_type, String p_lexeme, Optional<String> p_literal, int p_line)
			: m_type(p_type), m_lexeme(p_lexeme), m_literal(p_literal), m_line(p_line)
		{
		}

		String TokenTypeToString()
		{
			switch (m_type)
			{
				CASE_TOKEN(SEMICOLON)
				CASE_TOKEN(NEWLINE)
				CASE_TOKEN(LPAREN)
				CASE_TOKEN(RPAREN)
				CASE_TOKEN(NUMBER)
				CASE_TOKEN(FLOAT_NUMBER)
				CASE_TOKEN(INT8)
				CASE_TOKEN(INT16)
				CASE_TOKEN(INT32)
				CASE_TOKEN(FLOAT)
				CASE_TOKEN(DOUBLE)
				CASE_TOKEN(PUSH)
				CASE_TOKEN(POP)
				CASE_TOKEN(DUMP)
				CASE_TOKEN(ASSERT)
				CASE_TOKEN(ADD)
				CASE_TOKEN(SUB)
				CASE_TOKEN(MUL)
				CASE_TOKEN(DIV)
				CASE_TOKEN(MOD)
				CASE_TOKEN(PRINT)
				CASE_TOKEN(EXIT)
				default:
					return "";
			}
		}

		String ToString()
		{
			return "{ m_type: " + TokenTypeToString()
				+ ", m_lexeme: " + m_lexeme
				+ ", m_literal: " + fmt::format("{}", m_literal.value_or("(null)"))
				+ ", m_line:" + std::to_string(m_line) + " }";
		}
	};

	class Scanner {
		public:
			Scanner(StringView p_input);
			List<Token> ScanTokens();

		private:
			bool IsAtEnd();
			void ScanCurrentToken();
			char Advance();
			void AddToken(TokenType);
			void AddToken(TokenType, Optional<String>);
			bool Match(char p_expected);
			char Peek();
			char PeekNext(StringView::size_type n);
			char IsAlpha(char);
			char IsDigit(char);
			char IsAlphaNumeric(char);
			void Identifier();
			void Number();

		private:
			String m_source;
			List<Token> m_tokens;

			StringView::size_type m_start;
			StringView::size_type m_current;
			StringView::size_type m_line;

			inline static const UnorderedMap<String, TokenType> s_keywords = {
				{   "push", PUSH },
				{    "pop", POP },
				{   "dump", DUMP },
				{    "add", ADD },
				{    "sub", SUB },
				{    "mul", MUL },
				{    "div", DIV },
				{    "mod", MOD },
				{   "int8", INT8 },
				{  "int16", INT16 },
				{  "int32", INT32 },
				{  "float", FLOAT },
				{ "double", DOUBLE },
				{ "assert", ASSERT },
				{  "print", PRINT },
				{   "exit", EXIT },
			};
	};

	class Lexer {
		public:
			static void RunFile(StringView p_path)
			{
				std::fstream l_file;
				l_file.open(std::string(p_path), std::ios::in);

				std::ostringstream l_stringStream;
				l_stringStream << l_file.rdbuf();

				Run(l_stringStream.str());
			}

			static void Run(StringView p_source)
			{
				Scanner l_scanner(p_source);
				List<Token> l_tokens = l_scanner.ScanTokens();

				//for (Token &l_t: l_tokens)
				//{
				//	fmt::print("{}\n", l_t.ToString());
				//}

				s_tokens = l_tokens;
			}

			static void Error(int p_line, StringView p_message)
			{
				Report(p_line, "", p_message);
			}

			static void Error(Token p_token, String p_message)
			{
				if (p_token.m_type == TokenType::INPUT_STOP)
				{
					Report(p_token.m_line, "at end", p_message);
				}
				else
				{
					Report(p_token.m_line, " at '" + p_token.m_lexeme + "'", p_message);
				}
			}

			static void Report(int p_line, StringView p_where, StringView p_message)
			{
				fmt::print("[line {}] Error {}: {}\n", p_line, p_where, p_message);
				s_hadError = true;
			}

			static bool HadError() { return s_hadError; }
			static List<Token> const &GetTokens() { return s_tokens; }

		private:
			inline static bool s_hadError = false;
			inline static List<Token> s_tokens;
	};
}
