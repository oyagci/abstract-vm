#include "Interpreter.hpp"

namespace avm {

	Interpreter::~Interpreter()
	{
	}

	bool Interpreter::Evaluate(ast::Instruction const &p_instruction)
	{
		if (m_shouldExit)
		{
			return m_shouldExit;
		}

		p_instruction.Accept(*this);

		return m_shouldExit;
	}

	void Interpreter::VisitInstruction(ast::Instruction const &p_instruction)
	{
		ast::Instruction::Type const &l_type = p_instruction.GetType();

		UniquePtr<IOperand const> l_lhs = nullptr;
		UniquePtr<IOperand const> l_rhs = nullptr;
		IOperand const *l_result = nullptr;

		static const UnorderedMap<ast::Instruction::Type, std::function<IOperand const *(IOperand const &, IOperand const &)>>
			l_operandFnLookUp {
			{ ast::Instruction::Type::ADD, [] (IOperand const &p_lhs, IOperand const &p_rhs) { return p_lhs + p_rhs; } },
			{ ast::Instruction::Type::SUB, [] (IOperand const &p_lhs, IOperand const &p_rhs) { return p_lhs - p_rhs; } },
			{ ast::Instruction::Type::MUL, [] (IOperand const &p_lhs, IOperand const &p_rhs) { return p_lhs * p_rhs; } },
			{ ast::Instruction::Type::DIV, [] (IOperand const &p_lhs, IOperand const &p_rhs) { return p_lhs / p_rhs; } },
			{ ast::Instruction::Type::MOD, [] (IOperand const &p_lhs, IOperand const &p_rhs) { return p_lhs % p_rhs; } },
		};
		static const UnorderedMap<ast::Instruction::Type, std::function<void()>> l_operandLookUpNoParam {
			{ ast::Instruction::Type::POP,   [&] () { Pop(); }  },
			{ ast::Instruction::Type::DUMP,  [&] () { Dump(); } },
			{ ast::Instruction::Type::PRINT, [&] () { Print(); }},
			{ ast::Instruction::Type::EXIT,  [&] () { Exit(); } },
		};

		if (l_operandFnLookUp.find(l_type) != l_operandFnLookUp.end())
		{
			if (m_stack.size() < 2)
			{
				throw EmptyStackError();
			}

			l_rhs = std::move(m_stack.back());
			m_stack.pop_back();

			l_lhs = std::move(m_stack.back());
			m_stack.pop_back();

			l_result = l_operandFnLookUp.at(l_type)(*l_lhs, *l_rhs);

			m_stack.emplace_back(UniquePtr<IOperand const>(l_result));
		}
		else if (l_operandLookUpNoParam.find(l_type) != l_operandLookUpNoParam.end())
		{
			l_operandLookUpNoParam.at(l_type)();
		}
	}

	void Interpreter::VisitInstructionWithValue(ast::InstructionWithValue const &p_instruction)
	{
		switch (p_instruction.GetType())
		{
			case ast::Instruction::Type::PUSH:
				PushValueToStack(*p_instruction.GetValue());
				break;
			case ast::Instruction::Type::ASSERT:
				Assert(*p_instruction.GetValue());
				break;
			default:
				throw std::runtime_error("Unreachable!");
		};
	}

	bool Interpreter::HasExited() const
	{
		return m_shouldExit;
	}

	eOperandType Interpreter::StringToOperandType(String const &l_str) const
	{
		static const UnorderedMap<String, eOperandType> l_lookUp {
			{ "int8",   eOperandType::INT8   },
			{ "int16",  eOperandType::INT16  },
			{ "int32",  eOperandType::INT32  },
			{ "float",  eOperandType::FLOAT  },
			{ "double", eOperandType::DOUBLE },
		};

		return l_lookUp.at(l_str);
	}

	eOperandType Interpreter::TokenTypeToOperandType(TokenType p_type)
	{
		static const UnorderedMap<TokenType, eOperandType> l_lookUp {
			{ TokenType::INT8,   eOperandType::INT8   },
			{ TokenType::INT16,  eOperandType::INT16  },
			{ TokenType::INT32,  eOperandType::INT32  },
			{ TokenType::FLOAT,  eOperandType::FLOAT  },
			{ TokenType::DOUBLE, eOperandType::DOUBLE },
		};

		return l_lookUp.at(p_type);
	}

	void Interpreter::PushValueToStack(ast::Value const &p_value)
	{
		String const &l_literal = *p_value.GetToken().m_literal;
		String const &l_type = p_value.GetType().m_lexeme;
		UniquePtr<IOperand const> l_stackValue(OperandFactory::Get().CreateOperand(
			StringToOperandType(l_type), l_literal));

		m_stack.push_back(std::move(l_stackValue));
	}

	void Interpreter::Pop()
	{
		if (m_stack.empty())
		{
			throw EmptyStackError();
		}
		m_stack.pop_back();
	}

	void Interpreter::Dump() const
	{
		for (auto l_stackVal = m_stack.rbegin(); l_stackVal != m_stack.rend(); l_stackVal++)
		{
			fmt::print("{}\n", (*l_stackVal)->toString());
		}
	}

	void Interpreter::Print() const
	{
		if (m_stack.empty())
		{
			throw EmptyStackError();
		}

		Operand<int8_t> const *l_operand = dynamic_cast<Operand<int8_t> const *>(m_stack.back().get());

		if (l_operand != nullptr)
		{
			fmt::print("{}", (char)l_operand->GetValue());
		}
		else
		{
			throw PrintError();
		}
	}

	void Interpreter::Assert(ast::Value const &p_value)
	{
		if (m_stack.empty())
		{
			throw EmptyStackError();
		}

		UniquePtr<IOperand const> &l_assertion = m_stack.back();

		if (TokenTypeToOperandType(p_value.GetType().m_type) != l_assertion->getType())
		{
			throw AssertError();
		}

		IOperand const *l_value = OperandFactory::Get().CreateOperand(
			StringToOperandType(p_value.GetType().m_lexeme),
			p_value.GetToken().m_lexeme);

		if (*l_assertion != *l_value)
		{
			throw AssertError();
		}
	}

	void Interpreter::Exit()
	{
		m_shouldExit = true;
	}

	// Exceptions
	// ==========

	InterpreterError::InterpreterError() : std::runtime_error("Interpreter Error")
	{
	}

	char const *InterpreterError::what() const noexcept
	{
		return "Interpreter error";
	}

	char const *EmptyStackError::what() const noexcept
	{
		return "Stack is empty";
	}

	char const *AssertError::what() const noexcept
	{
		return "Assertion failed";
	}

	char const *PrintError::what() const noexcept
	{
		return "Value is not of type int8";
	}
}
