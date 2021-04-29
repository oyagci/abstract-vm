#pragma once
#include "../abstractvm.hpp"
#include "../Lexer.hpp"
#include "Value.hpp"

namespace avm {
namespace ast {

	class Instruction;
	class InstructionWithValue;

	class InstructionVisitor
	{
	public:
		virtual ~InstructionVisitor() {}

		virtual void VisitInstruction(Instruction const &) {}
		virtual void VisitInstructionWithValue(InstructionWithValue const &) {}
	};

	class InstructionVisitee
	{
	public:
		virtual ~InstructionVisitee() {}

		virtual void Accept(InstructionVisitor &) const = 0;
	};

	class Instruction : public InstructionVisitee
	{
	public:
		enum class Type
		{
			PUSH,
			POP,
			DUMP,
			ASSERT,
			ADD,
			SUB,
			MUL,
			DIV,
			MOD,
			PRINT,
			EXIT,
		};

	public:
		Instruction() = delete;
		Instruction(Type p_type);
		Instruction(const Instruction &) = delete;
		virtual ~Instruction() = default;

		Instruction &operator=(const Instruction &) = delete;

		Type GetType() const;

		virtual void Print() const;

		void Accept(InstructionVisitor &p_visitor) const override;

	protected:
		Type const m_type;
	};

	class InstructionWithValue : public Instruction
	{
	public:
		InstructionWithValue() = delete;
		InstructionWithValue(Instruction::Type p_type, UniquePtr<Value const> p_value);
		InstructionWithValue(const InstructionWithValue &) = delete;

		InstructionWithValue &operator=(const InstructionWithValue &) = delete;

		virtual ~InstructionWithValue() = default;

		virtual Value const *GetValue() const;

		void Print() const override;

		void Accept(InstructionVisitor &p_visitor) const override;

	protected:
		UniquePtr<Value const> m_value;
	};

	class Program
	{
	public:
		Program() = default;
		Program(const Program &) = delete;
		virtual ~Program() = default;

		Program &operator=(const Program &) = delete;

		void AddInstruction(UniquePtr<Instruction const> p_instruction);

		UniquePtr<Instruction const> GetNextInstruction();

		void Print() const;

	private:
		List<UniquePtr<Instruction const>> m_instructions;
	};
}
}
