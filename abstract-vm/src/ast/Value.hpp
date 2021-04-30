#pragma once

#include "../Lexer.hpp"

namespace avm {
namespace ast {

	class Value
	{
	public:
		Value() = delete;
		Value(Token p_type, Token p_number);
		Value(const Value &);
		virtual ~Value() = default;

		Value &operator=(const Value &other);

		Token const &GetType() const;
		Token const &GetToken() const;

		void Print() const;

	private:
		Token m_type;
		Token m_number;
	};

}
}
