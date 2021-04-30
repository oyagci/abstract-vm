#pragma once
#include "ast/Instruction.hpp"
#include "Operand.hpp"

namespace avm {

	class Interpreter : public ast::InstructionVisitor
	{
	public:
		Interpreter() = default;
		Interpreter(const Interpreter &) = delete;
		virtual ~Interpreter();

		Interpreter &operator=(const Interpreter &) = delete;

		bool Evaluate(ast::Instruction const &p_instruction);
		void VisitInstruction(ast::Instruction const &p_instruction) override;
		void VisitInstructionWithValue(ast::InstructionWithValue const &p_instruction) override;

		bool HasExited() const;

	private:
		eOperandType StringToOperandType(String const &l_str) const;
		eOperandType TokenTypeToOperandType(TokenType p_type);

		void PushValueToStack(ast::Value const &p_value);
		void Pop();
		void Dump() const;
		void Print() const;
		void Assert(ast::Value const &p_value);
		void Exit();

	private:
		Vector<UniquePtr<IOperand const>> m_stack;
		bool m_shouldExit = false;
	};

	// Exceptions
	// ==========

	class InterpreterError : public std::runtime_error
	{
	public:
		InterpreterError();

		char const *what() const noexcept override;
	};

	class EmptyStackError : public InterpreterError
	{
	public:
		char const *what() const noexcept override;
	};

	class AssertError : public InterpreterError
	{
	public:
		char const *what() const noexcept override;
	};

	class PrintError : public InterpreterError
	{
	public:
		char const *what() const noexcept override;
	};
}
