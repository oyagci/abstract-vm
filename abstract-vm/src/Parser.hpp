#pragma once
#include "abstractvm.hpp"
#include "ast/Instruction.hpp"

namespace avm {

	class ParseError : public std::runtime_error
	{
	public:
		ParseError();

		char const *what() const noexcept override;
	};

	class Lexer;

	class Parser
	{
		public:
			Parser() = delete;
			Parser(Lexer &p_lexer, List<Token> p_tokens);
			Parser(const Parser &) = delete;
			~Parser() = default;

			Parser &operator=(const Parser &) = delete;

			UniquePtr<ast::Program> Run();

		private:
			UniquePtr<ast::Program> Program();
			UniquePtr<ast::Instruction const> Instruction();
			UniquePtr<ast::Value const> Value();

			// Compile-time array with correct values
			// Student project, just for learning, etc...
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

			template <TokenType... Ts>
			bool Match()
			{
				TokenType l_result = TokenType::NONE;
				(void)l_result;
				return Match<Ts...>(l_result);
			}

			bool Check(TokenType p_type) const;

			template <TokenType... Ts>
			bool MatchSeq()
			{
				if ((CheckAdvance(Ts) && ...))
				{
					return true;
				}
				return false;
			}

			bool CheckAdvance(TokenType p_type);
			bool IsAtEnd(size_t p_n = 0) const;
			Token const &Peek() const;
			Token const &Previous(size_t p_n = 1) const;
			Token const &Advance(size_t p_n = 1);
			Token const &At(size_t p_idx) const;
			Token Consume(TokenType p_type, String p_message);
			ParseError Error(Token p_token, String p_message) const;
			void Synchronize();

		private:
			Lexer &m_lexer;
			List<Token> m_tokens;
			size_t m_current;
	};
}
