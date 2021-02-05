#include "Lexer.hpp"

namespace avm {
	Scanner::Scanner(StringView p_input)
		: m_source(p_input), m_start(0), m_current(0), m_line(1)
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
					Lexer::Error(m_line, fmt::format("Unexpected Character: '{}'", l_ch));
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
			Lexer::Error(m_line, fmt::format("Unexpected Identifier: '{}'", l_text));
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
}
