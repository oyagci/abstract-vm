#pragma once
#include "abstractvm.hpp"
#include "ast/Instruction.hpp"

namespace avm {

	class ParseError : public std::runtime_error
	{
	public:
		ParseError() : std::runtime_error("Parse Error")
		{
		}

		char const *what() const noexcept
		{
			return "Parse Error";
		}
	};

	class Parser
	{
		public:
			Parser() = delete;
			Parser(List<Token> p_tokens) : m_tokens(p_tokens), m_current(0)
			{
			}

			UniquePtr<ast::Program> Run()
			{
				return Program();
			}

		private:
			UniquePtr<ast::Program> Program()
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

			UniquePtr<ast::Instruction const> Instruction()
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

			UniquePtr<ast::Value const> Value()
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

			template <TokenType... Ts>
			bool Match(TokenType &p_result)
			{
				p_result = TokenType::NONE;

				Array<TokenType, sizeof...(Ts)> l_tokenTypes{ Ts... };

				for (auto const &l_t : l_tokenTypes)
				{
					if (Check(l_t))
					{
						Advance();
						p_result = l_t;
						return true;
					}
				}
				return false;
			}

			// Compile-time array with correct values
			// Student project, just for learning, etc...
			template <TokenType... Ts>
			bool Match()
			{
				TokenType l_result = TokenType::NONE;
				(void)l_result;
				return Match<Ts...>(l_result);
			}

			bool Check(TokenType p_type) const
			{
				if (IsAtEnd())
					return false;

				return Peek().m_type == p_type;
			}

			template <TokenType... Ts>
			bool MatchSeq()
			{
				if ((CheckAdvance(Ts) && ...))
				{
					return true;
				}
				return false;
			}

			bool CheckAdvance(TokenType p_type)
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

			bool IsAtEnd(size_t p_n = 0) const
			{
				return m_current + p_n >= m_tokens.size();
			}

			Token const &Peek() const
			{
				return At(m_current);
			}

			Token const &Previous(size_t p_n = 1) const
			{
				return At(m_current - p_n);
			}

			Token const &Advance(size_t p_n = 1)
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

			Token const &At(size_t p_idx) const
			{
				decltype(m_tokens)::const_iterator l_it = std::next(m_tokens.begin(), p_idx);

				return *l_it;
			}

			Token Consume(TokenType p_type, String p_message)
			{
				if (Check(p_type))
				{
					return Advance();
				}

				throw Error(Peek(), p_message);
			}

			ParseError Error(Token p_token, String p_message) const
			{
				Lexer::Error(p_token, p_message);
				throw ParseError();
			}

			void Synchronize()
			{
				while (!IsAtEnd() && Peek().m_type != TokenType::NEWLINE)
				{
					Advance();
				}
				Advance();
			}

		private:
			List<Token> m_tokens;
			size_t m_current;
	};
}
