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

		Token(TokenType p_type, String p_lexeme, Optional<String> p_literal, int p_line);
		String TokenTypeToString();
		String ToString();
	};

	class Lexer;

	class Scanner {
		public:
			Scanner() = delete;
			Scanner(Lexer &p_lexer, StringView p_input);
			Scanner(const Scanner &) = delete;
			~Scanner() = default;

			Scanner &operator=(const Scanner &other) = delete;

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
			void NewLine();

		private:
			Lexer &m_lexer;
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
			Lexer() = default;
			Lexer(Lexer const &) = delete;
			Lexer(Lexer const &&) = delete;
			virtual ~Lexer() = default;

			Lexer &operator=(Lexer const &) = delete;

			void RunFile(StringView p_path);
			void Run(StringView p_source);
			void Error(int p_line, StringView p_message);
			void Error(Token p_token, String p_message);
			void Report(int p_line, StringView p_where, StringView p_message);

			bool HadError() const;
			List<Token> const &GetTokens() const;

		private:
			bool m_hadError = false;
			List<Token> m_tokens;
	};
}
