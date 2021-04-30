#include "Lexer.hpp"

namespace avm {

	Token::Token(TokenType p_type, String p_lexeme, Optional<String> p_literal, int p_line)
		: m_type(p_type), m_lexeme(p_lexeme), m_literal(p_literal), m_line(p_line)
	{
	}

	String Token::TokenTypeToString()
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

	String Token::ToString()
	{
		return "{ m_type: " + TokenTypeToString()
			+ ", m_lexeme: " + m_lexeme
			+ ", m_literal: " + fmt::format("{}", m_literal.value_or("(null)"))
			+ ", m_line:" + std::to_string(m_line) + " }";
	}

	Scanner::Scanner(Lexer &p_lexer, StringView p_input)
		: m_lexer(p_lexer), m_source(p_input), m_start(0), m_current(0), m_line(1)
	{
	}

	auto Scanner::ScanTokens() -> List<Token>
	{
		List<Token> l_tokens;

		while (!IsAtEnd())
		{
			m_start = m_current;
			ScanCurrentToken();
		}

		AddToken(TokenType::INPUT_STOP);

		return m_tokens;
	}

	bool Scanner::IsAtEnd()
	{
		return m_current >= m_source.length();
	}

	void Scanner::ScanCurrentToken()
	{
		char l_ch = Advance();
		switch (l_ch)
		{
			case '(':
				AddToken(LPAREN);
				break;
			case ')':
				AddToken(RPAREN);
				break;
			case ';':
				// Comment: skip until new line
				while (!IsAtEnd() && Peek() != '\n') Advance();
				break;
			case ' ':
			case '\t':
			case '\r':
				break;
			case '\n':
				m_line++;
				NewLine();
				break;
			default:
				if (IsAlpha(l_ch))
				{
					Identifier();
				}
				else if (IsDigit(l_ch) || l_ch == '-')
				{
					Number();
				}
				else
				{
					m_lexer.Error(m_line, fmt::format("Unexpected Character: '{}'", l_ch));
				}
				break;
		}
	}

	char Scanner::Advance()
	{
		m_current++;
		return m_source[m_current - 1];
	}

	void Scanner::AddToken(TokenType p_tokenType)
	{
		AddToken(p_tokenType, NullOpt);
	}

	void Scanner::AddToken(TokenType p_tokenType, Optional<String> p_literal)
	{
		String l_text = m_source.substr(m_start, m_current - m_start);
		m_tokens.emplace_back(Token(p_tokenType, l_text, p_literal, m_line));
	}

	bool Scanner::Match(char p_expected)
	{
		if (IsAtEnd()) return false;
		if (m_source.at(m_current) != p_expected) return false;

		m_current++;
		return true;
	}

	char Scanner::Peek()
	{
		if (IsAtEnd())
			return '\0';
		return m_source.at(m_current);
	}

	char Scanner::PeekNext(StringView::size_type n = 1)
	{
		if (m_current + n >= m_source.length())
			return '\0';
		return m_source.at(m_current + n);
	}

	char Scanner::IsAlpha(char p_char)
	{
		return (p_char >= 'A' && p_char <= 'Z') || (p_char >= 'a' && p_char <= 'z');
	}

	char Scanner::IsDigit(char p_char)
	{
		return p_char >= '0' && p_char <= '9';
	}

	char Scanner::IsAlphaNumeric(char p_char)
	{
		return IsAlpha(p_char) || IsDigit(p_char);
	}

	void Scanner::Identifier()
	{
		while (IsAlphaNumeric(Peek()))
			Advance();

		String l_text = m_source.substr(m_start, m_current - m_start);
		decltype(s_keywords)::const_iterator l_tokenType = s_keywords.find(l_text);
		if (l_tokenType != s_keywords.end())
		{
			AddToken(l_tokenType->second);
		}
		else
		{
			m_lexer.Error(m_line, fmt::format("Unexpected Identifier: '{}'", l_text));
		}
	}

	void Scanner::Number()
	{
		while (IsDigit(Peek()))
		{
			Advance();
		}

		if (Peek() == '.' && IsDigit(PeekNext()))
		{
			Advance();
			while (IsDigit(Peek()))
			{
				Advance();
			}
		}

		AddToken(NUMBER, m_source.substr(m_start, m_current - m_start));
	}

	void Scanner::NewLine()
	{
		AddToken(NEWLINE);

		while (!IsAtEnd() && Peek() == '\n') Advance();
	}

	void Lexer::RunFile(StringView p_path)
	{
		std::fstream l_file;
		l_file.open(std::string(p_path), std::ios::in);

		std::ostringstream l_stringStream;
		l_stringStream << l_file.rdbuf();

		Run(l_stringStream.str());
	}

	void Lexer::Run(StringView p_source)
	{
		m_hadError = false;

		Scanner l_scanner(*this, p_source);
		List<Token> l_tokens = l_scanner.ScanTokens();

		//for (Token &l_t: l_tokens)
		//{
		//	fmt::print("{}\n", l_t.ToString());
		//}

		m_tokens = l_tokens;
	}

	void Lexer::Error(int p_line, StringView p_message)
	{
		Report(p_line, "", p_message);
	}

	void Lexer::Error(Token p_token, String p_message)
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

	void Lexer::Report(int p_line, StringView p_where, StringView p_message)
	{
		fmt::print("[line {}] Error {}: {}\n", p_line, p_where, p_message);
		m_hadError = true;
	}

	bool Lexer::HadError() const
	{
		return m_hadError;
	}

	List<Token> const &Lexer::GetTokens() const
	{
		return m_tokens;
	}
}
