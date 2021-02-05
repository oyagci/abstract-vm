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
				while (42)
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
					}
					catch (ParseError const &l_e)
					{
						Synchronize();
					}

				}

				return l_program;
			}

			UniquePtr<ast::Instruction const> Instruction()
			{
				if (Match<TokenType::PUSH>() || Match<TokenType::ASSERT>())
				{
					Token const &l_instruction = Previous();
					UniquePtr<ast::Value const> l_value = Value();

					if (l_value)
					{
						if (l_instruction.m_type == TokenType::PUSH)
						{
							return MakeUnique<ast::InstructionWithValue>(ast::Instruction::InstructionType::PUSH, std::move(l_value));
						}
						else if (l_instruction.m_type == TokenType::ASSERT)
						{
							return MakeUnique<ast::InstructionWithValue>(ast::Instruction::InstructionType::ASSERT, std::move(l_value));
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

					ast::Instruction::InstructionType l_type;

					switch (l_instruction.m_type)
					{
					case TokenType::POP:
						l_type = ast::Instruction::InstructionType::POP;
						break;
					case TokenType::DUMP:
						l_type = ast::Instruction::InstructionType::DUMP;
						break;
					case TokenType::ADD:
						l_type = ast::Instruction::InstructionType::ADD;
						break;
					case TokenType::SUB:
						l_type = ast::Instruction::InstructionType::SUB;
						break;
					case TokenType::MUL:
						l_type = ast::Instruction::InstructionType::MUL;
						break;
					case TokenType::DIV:
						l_type = ast::Instruction::InstructionType::DIV;
						break;
					case TokenType::MOD:
						l_type = ast::Instruction::InstructionType::MOD;
						break;
					case TokenType::PRINT:
						l_type = ast::Instruction::InstructionType::PRINT;
						break;
					case TokenType::EXIT:
						l_type = ast::Instruction::InstructionType::EXIT;
						break;
					case TokenType::PUSH:
					case TokenType::ASSERT:
						// Must not happen
						throw std::runtime_error("Unreachable!");
					default:
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

			// Compile-time array with correct values
			// Student project, just for learning, etc...
			template <TokenType... Ts>
			bool Match()
			{
				Array<TokenType, sizeof...(Ts)> l_tokenTypes{ Ts... };

				for (auto const &l_t : l_tokenTypes)
				{
					if (Check(l_t))
					{
						Advance();
						return true;
					}
				}
				return false;
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
				return ParseError();
			}

			void Synchronize()
			{
				while (!IsAtEnd())
				{
					switch (Peek().m_type)
					{
						case TokenType::PUSH:
						case TokenType::POP:
						case TokenType::DUMP:
						case TokenType::ASSERT:
						case TokenType::ADD:
						case TokenType::SUB:
						case TokenType::MUL:
						case TokenType::DIV:
						case TokenType::MOD:
						case TokenType::PRINT:
							return ;
						default:
							break;
					}

					Advance();
				}
			}

		private:
			List<Token> m_tokens;
			size_t m_current;
	};
}
