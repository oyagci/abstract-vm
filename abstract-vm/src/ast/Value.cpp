#include "Instruction.hpp"

namespace avm {
namespace ast {

		Value::Value(Token p_type, Token p_number) : m_type(p_type), m_number(p_number)
		{
		}

		Value &Value::operator=(const Value &other)
		{
			m_type = other.m_type;
			m_number = other.m_number;

			return *this;
		}

		Token const &Value::GetType() const { return m_type; }
		Token const &Value::GetToken() const { return m_number; }

		void Value::Print() const
		{
			fmt::print("VALUE {} {}\n", m_type.m_lexeme, *m_number.m_literal);
		}

}
}
