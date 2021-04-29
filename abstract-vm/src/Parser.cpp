#include "Parser.hpp"

namespace avm {

	ParseError::ParseError() : std::runtime_error("Parse Error")
	{
	}

	char const *ParseError::what() const noexcept
	{
		return "Parse Error";
	}

	Parser::Parser(Lexer &p_lexer, List<Token> p_tokens) : m_lexer(p_lexer), m_tokens(p_tokens), m_current(0)
	{
	}

	UniquePtr<ast::Program> Parser::Run()
	{
		return Program();
	}

	UniquePtr<ast::Program> Parser::Program()
	{
		auto l_program = MakeUnique<ast::Program>();

		UniquePtr<ast::Instruction const> l_current = nullptr;
		do
		{
			try
			{
				l_current = Instruction();
				if (l_current != nullptr)
				{
					l_program->AddInstruction(std::move(l_current));
				}
				else
				{
					break;
				}

				if (IsAtEnd() || Match<TokenType::INPUT_STOP>())
				{
					break;
				}
				else if (!Match<TokenType::NEWLINE>())
				{
					Error(Peek(), "Expected newline");
					throw ParseError();
				}
			}
			catch (ParseError const &l_e)
			{
				Synchronize();
			}

		}
		while (42);

		return l_program;
	}

	UniquePtr<ast::Instruction const> Parser::Instruction()
	{
		while (Match<TokenType::NEWLINE>()) {}

		if (Match<TokenType::PUSH>() || Match<TokenType::ASSERT>())
		{
			Token const &l_instruction = Previous();
			UniquePtr<ast::Value const> l_value = Value();

			if (l_value)
			{
				if (l_instruction.m_type == TokenType::PUSH)
				{
					return MakeUnique<ast::InstructionWithValue>(ast::Instruction::Type::PUSH, std::move(l_value));
				}
				else if (l_instruction.m_type == TokenType::ASSERT)
				{
					return MakeUnique<ast::InstructionWithValue>(ast::Instruction::Type::ASSERT, std::move(l_value));
				}
				else
				{
					throw std::runtime_error("Unreachable!");
				}
			}
			else
			{
				Error(l_instruction, "Expected a number");
			}
		}
		else if (Match<
			TokenType::POP,
			TokenType::DUMP,
			TokenType::ADD,
			TokenType::SUB,
			TokenType::MUL,
			TokenType::DIV,
			TokenType::MOD,
			TokenType::PRINT,
			TokenType::EXIT>())
		{
			Token const &l_instruction = Previous();

			ast::Instruction::Type l_type;

			UnorderedMap<TokenType, ast::Instruction::Type> l_lookUpTable {
				{ TokenType::POP,   ast::Instruction::Type::POP   },
				{ TokenType::DUMP,  ast::Instruction::Type::DUMP  },
				{ TokenType::ADD,   ast::Instruction::Type::ADD   },
				{ TokenType::SUB,   ast::Instruction::Type::SUB   },
				{ TokenType::MUL,   ast::Instruction::Type::MUL   },
				{ TokenType::DIV,   ast::Instruction::Type::DIV   },
				{ TokenType::MOD,   ast::Instruction::Type::MOD   },
				{ TokenType::PRINT, ast::Instruction::Type::PRINT },
				{ TokenType::EXIT,  ast::Instruction::Type::EXIT  },
			};

			if (l_lookUpTable.find(l_instruction.m_type) != l_lookUpTable.end())
			{
				l_type = l_lookUpTable.at(l_instruction.m_type);
			}
			else if (l_instruction.m_type == TokenType::PUSH ||
					 l_instruction.m_type == TokenType::ASSERT)
			{
				throw std::runtime_error("Unreachable!");
			}
			else
			{
				throw std::runtime_error("Unreachable!");
			}

			return MakeUnique<ast::Instruction>(l_type);
		}


		return nullptr;
	}

	UniquePtr<ast::Value const> Parser::Value()
	{
		if (Match<TokenType::INT8>() ||
			Match<TokenType::INT16>() ||
			Match<TokenType::INT32>() ||
			Match<TokenType::FLOAT>() ||
			Match<TokenType::DOUBLE>())
		{
			Token const &l_type = Previous();

			if (MatchSeq<TokenType::LPAREN, TokenType::NUMBER>())
			{
				Token const &l_number = Previous();

				Consume(TokenType::RPAREN, "Expected \")\" after number.");

				return MakeUnique<ast::Value>(l_type, l_number);
			}
		}

		return nullptr;
	}

	bool Parser::Check(TokenType p_type) const
	{
		if (IsAtEnd())
			return false;

		return Peek().m_type == p_type;
	}

	bool Parser::CheckAdvance(TokenType p_type)
	{
		if (IsAtEnd())
			return false;

		if (Peek().m_type == p_type)
		{
			Advance();
			return true;
		}

		return false;
	}

	bool Parser::IsAtEnd(size_t p_n) const
	{
		return m_current + p_n >= m_tokens.size();
	}

	Token const &Parser::Peek() const
	{
		return At(m_current);
	}

	Token const &Parser::Previous(size_t p_n) const
	{
		return At(m_current - p_n);
	}

	Token const &Parser::Advance(size_t p_n)
	{
		if (p_n <= 0)
		{
			return At(m_current);
		}

		if (!IsAtEnd() && !IsAtEnd(p_n - 1))
		{
			m_current += p_n;
		}
		return Previous();
	}

	Token const &Parser::At(size_t p_idx) const
	{
		decltype(m_tokens)::const_iterator l_it = std::next(m_tokens.begin(), p_idx);

		return *l_it;
	}

	Token Parser::Consume(TokenType p_type, String p_message)
	{
		if (Check(p_type))
		{
			return Advance();
		}

		throw Error(Peek(), p_message);
	}

	ParseError Parser::Error(Token p_token, String p_message) const
	{
		m_lexer.Error(p_token, p_message);
		throw ParseError();
	}

	void Parser::Synchronize()
	{
		while (!IsAtEnd() && Peek().m_type != TokenType::NEWLINE)
		{
			Advance();
		}
		Advance();
	}
}
